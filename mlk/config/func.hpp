#ifndef INCLUDED_MLK_CONFIG_FUNC_HPP
#define INCLUDED_MLK_CONFIG_FUNC_HPP
#include "core.hpp"
#include "../type_traits/core.hpp"

namespace mlk {

// Function
template <template <class...> class F>
struct fn : show_type<fn<F>> {
    template <class... Args>
    struct exec
        : F<Args...> {};
};

namespace details {

template <class F, class... Args>
class exec_base {
    template <class G, MLK_REQUIRES_DEFAULT_TP>
    struct exec_impl {
        static_assert(
            type_traits::always_false<G>::value,
            "Type F must satisfy is_metafunc"
        );
    };
    template <class G>
    struct exec_impl<G, MLK_REQUIRES(type_traits::is_func<G>)>
        : def_type<typename F::template exec<Args...>> {};
public:
    typedef typename exec_impl<F>::type type;
};

template <class T, MLK_REQUIRES_DEFAULT_TP>
struct eval_base
    : def_type<T> {};

template <class T>
struct eval_base<T, MLK_REQUIRES(type_traits::is_metafunc<T>)>
    : T {}; 

template <class T, MLK_REQUIRES_DEFAULT_TP>
struct eval_rec_base
    : def_type<T> {};

template <class T>
struct eval_rec_base<T, MLK_REQUIRES(type_traits::is_metafunc<T>)>
    : eval_rec_base<typename T::type> {};

} // namespace details

template <class T>
using eval =
    typename details::eval_base<T>::type;

template <class T>
using eval_rec =
    typename details::eval_rec_base<T>::type;

template <class F, class... Args>
using exec =
    typename details::exec_base<F, Args...>::type;

} // namespace mlk

#endif
