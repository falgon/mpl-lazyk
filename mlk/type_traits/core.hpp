#ifndef INCLUDED_MLK_TYPE_TRAITS_CORE_HPP
#define INCLUDED_MLK_TYPE_TRAITS_CORE_HPP
#include "../config/core.hpp"

namespace mlk::type_traits {

template <template <class...> class...>
using void_fn_t = void;

template <class...>
using always_false = std::false_type;

template <class Lhs, class Rhs>
struct eq
    : std::bool_constant<Lhs::value == Rhs::value> {};

template <class Lhs, class Rhs>
struct lt 
    : std::bool_constant<Lhs::value < Rhs::value> {};

template <class Lhs, class Rhs>
struct gt 
    : std::conjunction<
        std::negation<lt<Lhs, Rhs>>, 
        std::negation<eq<Lhs, Rhs>>
      > {};

#define DEF_ACCESSIBLE(FN, TYPE_TRAITS_NAME) \
    template <class T>\
    class TYPE_TRAITS_NAME {\
        template <class, class = void>\
        struct TYPE_TRAITS_NAME##_base\
            : std::false_type {};\
        template <class U>\
        struct TYPE_TRAITS_NAME##_base<U, FN(U)>\
            : std::true_type {};\
    public:\
        inline static constexpr bool value =\
            TYPE_TRAITS_NAME##_base<T>::value;\
    }

// Metafunction constraints
#define DEF_METAFUNC_FN(U) std::void_t<typename U::type>
DEF_ACCESSIBLE(DEF_METAFUNC_FN, is_metafunc);
#undef DEF_METAFUNC_FN

// Function constraints
#define DEF_IS_FUNC_FN(U) mlk::type_traits::void_fn_t<U::template exec>
DEF_ACCESSIBLE(DEF_IS_FUNC_FN, is_func);
#undef DEF_IS_FUNC_FN

} // namespace mlk::type_traits

#endif
