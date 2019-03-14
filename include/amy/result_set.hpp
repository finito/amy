#ifndef __AMY_RESULT_SET_HPP__
#define __AMY_RESULT_SET_HPP__

#include <amy/detail/mysql_types.hpp>
#include <amy/detail/throw_error.hpp>

#include <amy/field_info.hpp>
#include <amy/row.hpp>

#include <memory>

namespace amy {

/// Provides MySQL query result set functionality.
/**
 * The \c result_set class wraps the underlying \c MYSQL_RES* pointer returned
 * by a \c mysql_store_result() call. It also provides STL compatible random
 * access iterator over rows in the result set.
 */
class result_set {
private:
    typedef std::vector<row> values_type;

    typedef
        values_type::const_iterator
        values_const_iterator;

    typedef
        values_type::const_reverse_iterator
        values_const_reverse_iterator;

	struct result_set_deleter {
		void operator()(void* p) {
			namespace ops = detail::mysql_ops;

			if (!!p) {
				ops::mysql_free_result(static_cast<detail::result_set_handle>(p));
			}
		}
	};

public:
    /// The random access iterator over rows.
    typedef values_const_iterator const_iterator;

    /// The random access reverse iterator over rows.
    typedef values_const_reverse_iterator const_reverse_iterator;

    /// The native representation of a MySQL query result set.
    typedef detail::result_set_handle native_type;

    typedef detail::mysql_handle native_mysql_type;

    typedef std::vector<field_info> fields_info_type;

    explicit result_set() :
		row_count_(0),
		affected_rows_(0),
		field_count_(0),
		result_set_(static_cast<detail::result_set_handle>(nullptr),
			result_set_deleter()),
        fields_info_(new fields_info_type)
    {}

    result_set(result_set const& other) :
		row_count_(other.row_count_),
		affected_rows_(other.affected_rows_),
		field_count_(other.field_count_),
        result_set_(other.result_set_),
        values_(other.values_),
        fields_info_(other.fields_info_)
    {}

    result_set const& operator=(result_set const& other) {
		row_count_ = other.row_count_,
		affected_rows_ = other.affected_rows_,
		field_count_ = other.field_count_,
        result_set_ = other.result_set_;
        values_ = other.values_;
        fields_info_ = other.fields_info_;
        return *this;
    }

    void assign(native_mysql_type mysql)
    {
        AMY_SYSTEM_NS::error_code ec;
        assign(mysql, ec);
        detail::throw_error(ec, mysql);
    }

    void assign(
            native_mysql_type mysql,
            AMY_SYSTEM_NS::error_code& ec)
    {
        namespace ops = amy::detail::mysql_ops;

		result_set_.reset(
			ops::mysql_store_result(mysql, ec),
			result_set_deleter());

        if (!result_set_) {
            return;
        }

        row_count_ = ops::mysql_num_rows(result_set_.get());

        if (row_count_ == 0) {
            return;
        }

        // Fetch fields information.
        field_count_ = ops::mysql_num_fields(result_set_.get());
        fields_info_.reset(new fields_info_type);
        fields_info_->reserve(field_count_);
        detail::field_handle f = nullptr;

        while ((f = ops::mysql_fetch_field(result_set_.get()))) {
            fields_info_->push_back(field_info(f));
        }

        // Fetch rows.
        values_.reserve(static_cast<size_t>(row_count_));
        detail::row_type r;

        while ((r = ops::mysql_fetch_row(mysql, result_set_.get(), ec))) {
            unsigned long* lengths = ops::mysql_fetch_lengths(result_set_.get());
            values_.push_back(row(result_set_.get(), r, lengths, fields_info_));
        }

        if (ec) {
			row_count_ = 0;
            values_.clear();
            fields_info_.reset();
            result_set_.reset();
        } else {
			affected_rows_ = detail::mysql_ops::mysql_affected_rows(mysql);
		}

        return;
    }

    static result_set empty_set() {
        return result_set();
    }

    const_iterator begin() const {
        return values_.begin();
    }

    const_iterator end() const {
        return values_.end();
    }

    const_reverse_iterator rbegin() const {
        return values_.rbegin();
    }

    const_reverse_iterator rend() const {
        return values_.rend();
    }

    bool empty() const {
        return !result_set_.get() || !size();
    }

    uint64_t size() const {
        return row_count_;
    }

    row const& operator[](const_iterator::difference_type index) const {
        return values_.at(index);
    }

    row const& at(const_iterator::difference_type index) const {
        return values_.at(index);
    }

    native_type native() const {
        return result_set_.get();
    }

    fields_info_type const& fields_info() const {
        return *fields_info_;
    }

    uint32_t field_count() const {
        return field_count_;
    }

    uint64_t affected_rows() const {
        return affected_rows_;
    }

private:
	uint64_t row_count_;
	uint64_t affected_rows_;
	uint32_t field_count_;
    std::shared_ptr<detail::result_set_type> result_set_;
    values_type values_;
    std::shared_ptr<fields_info_type> fields_info_;

}; // class result_set

} // namespace amy

#endif // __AMY_RESULT_SET_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
