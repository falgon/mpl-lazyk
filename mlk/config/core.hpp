#ifndef INCLUDED_MLK_CONFIG_CORE_HPP
#define INCLUDED_MLK_CONFIG_CORE_HPP
#if defined(__cplusplus) && __cplusplus >= 201703L
#include <array>
#include <algorithm>
#include <ostream>
#include <type_traits>
#include <utility>
#if !__has_include(<experimental/iterator>)
#   error "make_ostream_joiner is not supported"
#else
#   include <experimental/iterator>
#   if !defined(__cpp_lib_experimental_ostream_joiner) ||\
        __cpp_lib_experimental_ostream_joiner < 201411
#       error "make_ostream_joiner is not supported"
#   endif
#endif
#if !__has_include(<boost/type_index.hpp>)
#   error "boost::typeindex must be available"
#else
#   include <boost/type_index.hpp>
#if !__has_include(<boost/preprocessor/repetition/repeat.hpp>) ||\
        !__has_include(<boost/preprocessor/punctuation/comma_if.hpp>)
#   error "boost preprocessor must be available"
#else
#   include <boost/preprocessor/repetition/repeat.hpp>
#   include <boost/preprocessor/punctuation/comma_if.hpp>
#endif
#endif

namespace mlk {

namespace details {

template <std::size_t len>
constexpr char get_ch(char const (&s)[len], std::size_t i)
{
    return i >= len ? '\0' : s[i];
}

} // namespace details

#define STRING_TO_CHARS_EXTRACT(z, n, data) \
    BOOST_PP_COMMA_IF(n) mlk::details::get_ch(data, n)

#define MLK_PP_CHARS(STRLEN, STR) \
    BOOST_PP_REPEAT(STRLEN, STRING_TO_CHARS_EXTRACT, STR)

template <class... Ts>
struct show_type {
    friend std::ostream& operator<<(std::ostream& os, const show_type&)
    {
        using boost::typeindex::type_id;
        using std::experimental::make_ostream_joiner;
        const std::array<std::string, sizeof...(Ts)> types { type_id<Ts>().pretty_name()... };
        std::copy(std::cbegin(types), std::cend(types), make_ostream_joiner(os, ", "));
        return os;
    }
};

struct unit : show_type<unit> {
    template <template <class...> class>
    using transfer = unit;
};


template <class T>
struct def_type
    : std::enable_if<true, T>, show_type<def_type<T>> {};

#define MLK_REQUIRES_DEFAULT_TP class = std::nullptr_t
#define MLK_REQUIRES(REQ_TYPE) \
    std::enable_if_t<REQ_TYPE::value, std::nullptr_t>

template <class T, MLK_REQUIRES_DEFAULT_TP>
struct eval
    : def_type<T> {};

} // namespace mlk

#else
#   error "Only supports running on C++17"
#endif
#endif
