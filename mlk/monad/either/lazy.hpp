#ifndef INCLUDED_MLK_MONAD_EITHER_LAZY_HPP
#define INCLUDED_MLK_MONAD_EITHER_LAZY_HPP
#include "../../config.hpp"
#include "../../type_traits.hpp"

namespace mlk::monad::either::lazy {

template <class = unit, class = unit>
class either;

template <class L>
class either<L, unit> {
public:
    template <class>
    using bind = either<L, unit>;

    template <class X>
    using eta = either<unit, X>;

    inline static constexpr bool is_left = true;
    inline static constexpr bool is_right = !is_left;
};

template <class R>
class either<unit, R> {
    template <class K>
    class bind_result {
        static_assert(type_traits::is_func<K>::value);
    public:
        typedef typename K::template exec<R> type;
    };
public:
    template <class FM>
    using bind = typename bind_result<FM>::type::type;

    template <class X>
    using eta = either<unit, X>;

    inline static constexpr bool is_left = false;
    inline static constexpr bool is_right = !is_left;
};

template <class X>
using right = either<unit, X>;

template <class X>
using left = either<X, unit>;

template <class X>
using eta = right<X>;

template <>
class either<unit, unit> {
public:
    template <class X>
    using eta = right<X>;

    template <class>
    using bind = either<unit, unit>;
};

namespace details {

template <class, class, class>
struct either_caser;

template <class LF, class RF, class L>
struct either_caser<LF, RF, either<L, unit>>
    : LF::template exec<L> {};

template <class LF, class RF, class R>
struct either_caser<LF, RF, either<unit, R>>
    : RF::template exec<R> {};

template <class>
struct from_left_base;

template <>
struct from_left_base<either<unit, unit>>;

template <class L>
struct from_left_base<either<L, unit>>
    : def_type<L> {};

template <class>
struct from_right_base;

template <>
struct from_right_base<either<unit, unit>>;

template <class R>
struct from_right_base<either<unit, R>>
    : def_type<R> {};

} // namespace details

template <class LF, class RF, class Either>
using either_case = typename details::either_caser<LF, RF, Either>::type;

template <class Either>
using from_left = typename details::from_left_base<Either>::type;

template <class Either>
using from_right = typename details::from_right_base<Either>::type;

} // namespace mlk::monad::lazy::either
#endif

