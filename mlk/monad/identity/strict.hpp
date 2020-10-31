#ifndef INCLUDED_MLK_MONAD_IDENTITY_STRICT_HPP
#define INCLUDED_MLK_MONAD_IDENTITY_STRICT_HPP
#include "../../config.hpp"

namespace mlk::monad::identity::strict {

template <class X = unit>
struct identity {
    template <template <class> class F>
    using bind = typename F<X>::type;

    template <class T>
    using eta = identity<T>;
};

namespace details {

template <class>
struct run_identity_base;

template <class X>
struct run_identity_base<identity<X>>
    : def_type<X> {};

} // namespace details

template <class Identity>
using run_identity = typename details::run_identity_base<Identity>::type;

template <class X>
using eta = identity<X>;

} // namespace mlk::monad::identity::strict

#endif
