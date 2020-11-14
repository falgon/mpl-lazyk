#ifndef INCLUDED_MLK_TYPE_TRAITS_MONAD_STRICT_HPP
#define INCLUDED_MLK_TYPE_TRAITS_MONAD_STRICT_HPP
#include "../core.hpp"

namespace mlk::type_traits::strict {

namespace details {

template <template <template <class> class> class>
using void_nested2_t = void;

#define DEF_HAS_BIND_FN(U) void_nested2_t<U::template bind>
    DEF_ACCESSIBLE(DEF_HAS_BIND_FN, has_bind);
#undef DEF_HAS_BIND_FN

#define DEF_HAS_ETA_FN(U) mlk::type_traits::void_fn_t<U::template eta>
    DEF_ACCESSIBLE(DEF_HAS_ETA_FN, has_eta);
#undef DEF_HAS_ETA_FN

} // namespace details

template <class T>
struct is_monad
    : std::conjunction<details::has_bind<T>, details::has_eta<T>> {};

} // namespace mlk
#endif
