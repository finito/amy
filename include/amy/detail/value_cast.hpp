#ifndef __AMY_DETAIL_VALUE_CAST_HPP__
#define __AMY_DETAIL_VALUE_CAST_HPP__

#include <amy/sql_types.hpp>

#include <date/date.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <iomanip>
#include <string>
#include <sstream>

namespace amy {
namespace detail {

template<typename ValueType>
ValueType value_cast(const char* s, unsigned long l) {
    ValueType v{};
    std::istringstream is({s, l});
    is >> v;
    return v;
}

template<>
inline std::string value_cast(const char* s, unsigned long l) {
    return {s, l};
}

template<>
inline long long value_cast(const char* s, unsigned long) {
    return std::strtoll(s, NULL, 10);
}

template<>
inline long value_cast(const char* s, unsigned long) {
    return std::strtol(s, NULL, 10);
}

template<>
inline int32_t value_cast(const char* s, unsigned long) {
    return static_cast<int32_t>(std::strtol(s, NULL, 10));
}

template<>
inline int16_t value_cast(const char* s, unsigned long) {
    return static_cast<int16_t>(std::strtol(s, NULL, 10));
}

template<>
inline int8_t value_cast(const char* s, unsigned long) {
    return static_cast<int8_t>(std::strtol(s, NULL, 10));
}

template<>
inline bool value_cast(const char* s, unsigned long) {
    return static_cast<bool>(std::strtol(s, NULL, 10));
}

template<>
inline unsigned long value_cast(const char* s, unsigned long) {
    return std::strtoul(s, NULL, 10);
}

template<>
inline uint32_t value_cast(const char* s, unsigned long) {
    return static_cast<uint32_t>(std::strtoul(s, NULL, 10));
}

template<>
inline uint16_t value_cast(const char* s, unsigned long) {
    return static_cast<int16_t>(std::strtoul(s, NULL, 10));
}

template<>
inline uint8_t value_cast(const char* s, unsigned long) {
    return static_cast<uint8_t>(std::strtoul(s, NULL, 10));
}

template<>
inline double value_cast(const char* s, unsigned long) {
    return std::strtod(s, NULL);
}

template<>
inline float value_cast(const char* s, unsigned long) {
    return std::strtof(s, NULL);
}

template<>
inline sql_datetime value_cast(const char* s, unsigned long l) {
	std::stringstream ss{std::string(s, l)};
	sql_datetime v;
	ss >> date::parse("%F %T", v);
	if (ss.fail())
		throw std::runtime_error("datetime parse failed" + ss.str());
	return v;
}

template<>
inline sql_time value_cast(const char* s, unsigned long l) {
	std::stringstream ss{std::string(s, l)};
	sql_time v;
	ss >> date::parse("%T", v);
	if (ss.fail())
		throw std::runtime_error("time parse failed" + ss.str());
    return v;
}

#ifdef WIN32

#include <stdio.h> // For ::_strtoui64

/// \c value_cast<sql_bigint_unsigned> specialization for win32 platform only.
/**
 * \c std::istringstream cannot deal with unsigned 64-bit integer under the
 * win32 platform.  And \c _strtoui64 is used instead.
 */
template<>
inline sql_bigint_unsigned value_cast(const char* s, unsigned long) {
    sql_bigint_unsigned v = ::_strtoui64(s, NULL, 10);
    return v;
}

#else

template<>
inline unsigned long long value_cast(const char* s, unsigned long) {
    return std::strtoull(s, NULL, 10);
}

#endif // ifdef WIN32

} // namespace detail
} // namespace amy

#endif // __AMY_DETAIL_VALUE_CAST_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
