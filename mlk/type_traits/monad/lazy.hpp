#ifndef INCLUDED_MLK_TYPE_TRAITS_MONAD_LAZY_HPP
#define INCLUDED_MLK_TYPE_TRAITS_MONAD_LAZY_HPP
#include "../core.hpp"
#include "../../config/func.hpp"

namespace mlk::type_traits::lazy {

template <class X>
class is_monad {
#define DEF_HAS_ETA_FN(U) mlk::type_traits::void_fn_t<U::template eta>
    DEF_ACCESSIBLE(DEF_HAS_ETA_FN, has_eta);
#undef DEF_HAS_ETA_FN
#define DEF_HAS_BIND_FN(U) mlk::type_traits::void_fn_t<U::template bind>
    DEF_ACCESSIBLE(DEF_HAS_BIND_FN, has_bind);
#undef DEF_HAS_BIND_FN
public:
    inline static constexpr bool value =
        std::conjunction_v<has_eta<X>, has_bind<X>>;
};

} // namespace mlk::type_traits

#endif
