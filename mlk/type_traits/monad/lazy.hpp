#ifndef INCLUDED_MLK_TYPE_TRAITS_MONAD_LAZY_HPP
#define INCLUDED_MLK_TYPE_TRAITS_MONAD_LAZY_HPP
#include "../core.hpp"
#include "../../config/func.hpp"

namespace mlk::type_traits::lazy {

template <class X>
struct is_monad {
#define DEF_HAS_ETA_FN(U) mlk::type_traits::void_fn_t<U::template eta>
    DEF_ACCESSIBLE(DEF_HAS_ETA_FN, has_eta);
#undef DEF_HAS_ETA_FN
#define DEF_HAS_BIND_FN(U) mlk::type_traits::void_fn_t<U::template bind>
    DEF_ACCESSIBLE(DEF_HAS_BIND_FN, has_bind);
#undef DEF_HAS_BIND_FN

    template <class M>
    struct monad_laws {
        template <class XM>
        struct monad_law_components {
            template <class T>
            struct F
                : def_type<typename XM::template eta<T>> {};
            typedef fn<F> f_type;
            template <class T>
            struct G
                : def_type<typename F<T>::type::template bind<f_type>> {};
            typedef fn<G> g_type;
        };
        struct law1 {
            typedef
                typename M
                    ::template eta<unit>
                    ::template bind<typename monad_law_components<M>::f_type>
                lhs;
            typedef
                typename monad_law_components<M>::f_type
                    ::template exec<unit>
                rhs;
        public:
            inline static constexpr bool value =
                std::is_same_v<eval<lhs>, eval<rhs>>;
        };
        struct law2 {
            typedef
                typename M::template bind<typename monad_law_components<M>::f_type>
                lhs;
            typedef M rhs;
        public:
            inline static constexpr bool value =
                std::is_same_v<lhs, rhs>;
        };
        struct law3 {
            typedef
                typename M
                    ::template bind<typename monad_law_components<M>::f_type>
                    ::template bind<typename monad_law_components<M>::f_type>
                lhs;
            typedef
                typename M
                    ::template bind<typename monad_law_components<M>::g_type>
                rhs;
        public:
            inline static constexpr bool value =
                std::is_same_v<lhs, rhs>;
        };
    public:
        inline static constexpr bool value =
            std::conjunction_v<law1, law2, law3>;
    };
public:
    inline static constexpr bool value =
        std::conjunction_v<has_eta<X>, has_bind<X>, monad_laws<X>>;
};

} // namespace mlk::type_traits

#endif
