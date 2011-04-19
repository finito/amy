#ifndef __AMY_BASIC_CONNECTOR_HPP__
#define __AMY_BASIC_CONNECTOR_HPP__

#include <amy/auth_info.hpp>
#include <amy/client_flags.hpp>
#include <amy/detail/throw_error.hpp>
#include <amy/result_set.hpp>

#include <boost/config.hpp>
#include <boost/asio/basic_io_object.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/system_error.hpp>

namespace amy {

template<typename MySQLService>
class basic_connector : public boost::asio::basic_io_object<MySQLService> {
public:
    typedef MySQLService service_type;

    typedef typename service_type::native_type native_type;

    explicit basic_connector(boost::asio::io_service& io_service) :
        boost::asio::basic_io_object<MySQLService>(io_service)
    {}

    native_type native() {
        return this->service.native(this->implementation);
    }

    void open() {
        boost::system::error_code ec;
        detail::throw_error(open(ec), &(this->implementation.mysql));
    }

    boost::system::error_code open(boost::system::error_code& ec) {
        return this->service.open(this->implementation, ec);
    }

    bool is_open() const {
        return this->service.is_open(this->implementation);
    }

    void close() {
        return this->service.close(this->implementation);
    }

    std::string error_message(boost::system::error_code& ec) {
        return this->service.error_message(this->implementation, ec);
    }

    template<typename Endpoint>
    void connect(Endpoint const& endpoint,
                 auth_info const& auth,
                 std::string const& database,
                 client_flags flags)
    {
        boost::system::error_code ec;
        detail::throw_error(connect(endpoint, auth, database, flags, ec),
                            &(this->implementation.mysql));
    }

    template<typename Endpoint>
    boost::system::error_code connect(Endpoint const& endpoint,
                                      auth_info const& auth,
                                      std::string const& database,
                                      client_flags flags,
                                      boost::system::error_code& ec)
    {
        return this->service.connect(this->implementation,
                                     endpoint, auth, database, flags, ec);
    }

    void query(std::string const& stmt) {
        boost::system::error_code ec;
        detail::throw_error(query(stmt, ec), &(this->implementation.mysql));
    }

    boost::system::error_code query(std::string const& stmt,
                                    boost::system::error_code& ec)
    {
        return this->service.query(this->implementation, stmt, ec);
    }

    bool has_more_results() const {
        return this->service.has_more_results(this->implementation);
    }

    result_set store_result() {
        boost::system::error_code ec;
        result_set rs = store_result(ec);
        detail::throw_error(ec, &(this->implementation.mysql));
        return rs;
    }

    result_set store_result(boost::system::error_code& ec) {
        return this->service.store_result(this->implementation, ec);
    }

};  //  class basic_connector

}   //  namespace amy

#endif  //  __AMY_BASIC_CONNECTOR_HPP__

// vim:ft=cpp ts=4 sw=4 et