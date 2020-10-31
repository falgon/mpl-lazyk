#ifndef INCLUDED_MLK_MONAD_MAYBE_STRICT_HPP
#define INCLUDED_MLK_MONAD_MAYBE_STRICT_HPP
#include "../../config.hpp"
#include "../../type_traits.hpp"

namespace mlk::monad::maybe::strict {

template <class...>
struct maybe;

template <>
struct maybe<> {
    template <template <class> class>
    using bind = maybe<>;

    template <class T>
    using eta = maybe<T>;

    inline static constexpr bool is_just = false;
};

template <class T>
struct maybe<T> {
    template <template <class> class FM>
    using bind = typename FM<T>::type;

    template <class X>
    using eta = maybe<X>;

    inline static constexpr bool is_just = true;
};

namespace details {

template <class>
struct from_just_base;

template <class T>
struct from_just_base<maybe<T>>
    : def_type<T> {};

template <class, template <class> class, class>
struct maybe_case_base;

template <class N, template <class> class FJ, class T>
struct maybe_case_base<N, FJ, maybe<T>>
    : FJ<T> {};

template <class N, template <class> class FJ>
struct maybe_case_base<N, FJ, maybe<>>
    : def_type<N> {};

} // namespace details

template <class T>
using from_just = typename details::from_just_base<T>::type;

template <class T>
using just = maybe<T>;

using nothing = maybe<>;

template <class N, template <class> class FJ, class Maybe>
using maybe_case = typename details::maybe_case_base<N, FJ, Maybe>::type;

template <class T>
using eta = maybe<T>;

static_assert(type_traits::strict::is_monad<just<int>>::value);
static_assert(type_traits::strict::is_monad<nothing>::value);

} // namespace mlk::monad::maybe::strict
#endif

