#ifndef __AMY_DETAIL_MARIADB_TYPES_HPP__
#define __AMY_DETAIL_MARIADB_TYPES_HPP__

#include <amy/detail/mysql_types.hpp>

namespace amy {
namespace detail {

const int progress_callback          = MYSQL_PROGRESS_CALLBACK;
const int nonblock                   = MYSQL_OPT_NONBLOCK;

} // namespace detail
} // namespace amy

#endif // __AMY_DETAIL_MARIADB_TYPES_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
