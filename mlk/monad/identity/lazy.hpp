#ifndef INCLUDED_MLK_MONAD_IDENTITY_LAZY_HPP
#define INCLUDED_MLK_MONAD_IDENTITY_LAZY_HPP
#include "../../config.hpp"

namespace mlk::monad::identity::lazy {

template <class T = mlk::unit>
class identity {
    template <class K>
    struct bind_result
        : def_type<exec<K, T>> {};
public:
    template <class IdentityFM>
    using bind =
        typename bind_result<IdentityFM>::type::type;

    template <class X>
    using eta = 
        identity<X>;
};

namespace details {

template <class T>
struct identity_runner_base
    : def_type<T> {};

template <class T>
struct identity_runner_base<identity<T>>
    : identity_runner_base<T> {};

template <class>
struct identity_runner;

template <class T>
struct identity_runner<identity<T>>
    : identity_runner_base<identity<T>> {};

} // namespace details

template <class X>
using eta = identity<X>;

template <class Identity>
using run_identity = typename details::identity_runner<Identity>::type;

} // namespace mlk::monad::identity::lazy

#endif
