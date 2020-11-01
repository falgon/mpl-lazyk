#ifndef INCLUDED_MLK_APP_LAZYK_COMBINATORS_HPP
#define INCLUDED_MLK_APP_LAZYK_COMBINATORS_HPP
#include <mlk/data/tree.hpp>
#include <mlk/data/list/strict.hpp>

namespace mlk::lazyk {

template <class T>
struct dbi_elem 
    : mlk::def_type<T> {};

template <class>
struct lambda_elem;

template <class X, class... Xs>
struct lambda_elem<mlk::data::tree::tree<X, mlk::data::list::strict::list<Xs...>>>
    : mlk::def_type<mlk::data::tree::tree<X, mlk::data::list::strict::list<Xs...>>> {};

struct app_elem {};

template <std::size_t n>
using V = 
    mlk::data::tree::eta<dbi_elem<std::integral_constant<int, n>>>;

template <class Tree>
using L =
    mlk::data::tree::eta<lambda_elem<Tree>>;

template <class L, class R>
using A =
    mlk::data::tree::binary_tree<app_elem, L, R>;

using S = L<L<L<A<A<V<2>, V<0>>, A<V<1>, V<0>>>>>>;

using K = L<L<V<1>>>;

using I = L<V<0>>;

using U = L<A<A<V<0>, S>, K>>;

using B = L<L<L<A<A<V<2>, V<1>>, V<0>>>>>;

using zero = L<L<V<0>>>;

using suc = L<L<L<A<V<1>, A<A<V<2>, V<1>>, V<0>>>>>>;

using cons = L<L<L<A<A<V<0>, V<2>>, V<1>>>>>;

using car = L<A<V<0>, L<L<V<1>>>>>;

using cdr = L<A<V<0>, L<L<V<0>>>>>;

using nil = L<L<V<0>>>;

} // namespace mlk::lazyk

#endif
