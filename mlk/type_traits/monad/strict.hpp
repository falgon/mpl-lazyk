#ifndef INCLUDED_MLK_TYPE_TRAITS_MONAD_STRICT_HPP
#define INCLUDED_MLK_TYPE_TRAITS_MONAD_STRICT_HPP
#include "../core.hpp"

namespace mlk::type_traits::strict {

namespace details {

template <template <template <class> class> class>
using void_nested2_t = void;

template <class, class = void>
struct has_bind
    : std::false_type {};

template <class T>
struct has_bind<T, void_nested2_t<T::template bind>>
    : std::true_type {};

template <class, class = void>
struct has_eta
    : std::false_type {};

template <class T>
struct has_eta<T, void_fn_t<T::template eta>>
    : std::true_type {};

template <class M>
struct monad_law_base {
    template <class X>
    struct F
        : def_type<typename M::template eta<X>> {};

    template <class X>
    struct G 
        : def_type<typename F<X>::type::template bind<F>> {};
};

template <class M>
struct monad_law 
    : std::conjunction<
        std::is_same<
            typename M::template eta<unit>::template bind<monad_law_base<M>::template F>, 
            typename monad_law_base<M>::template F<unit>::type
        >,
        std::is_same<typename M::template bind<monad_law_base<M>::template F>, M>,
        std::is_same<
            typename M
                ::template bind<monad_law_base<M>::template F>
                ::template bind<monad_law_base<M>::template F>,
            typename M::template bind<monad_law_base<M>::template G>
        >
    > {};

} // namespace details

template <class T>
struct is_monad
    : std::conjunction<details::has_bind<T>, details::has_eta<T>, details::monad_law<T>> {};

} // namespace mlk
#endif
