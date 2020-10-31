#ifndef INCLUDED_MLK_DATA_TREE_CORE_HPP
#define INCLUDED_MLK_DATA_TREE_CORE_HPP
#include "../config.hpp"
#include "../data/list/strict.hpp"

namespace mlk::data::tree {

template <class, class>
struct tree;

template <class, class, class>
struct binary_tree;

namespace details {

template <class>
struct is_forest : std::false_type {};

template <class X, class... Forest>
struct is_forest<tree<X, mlk::data::list::strict::list<Forest...>>> : std::true_type {};

template <class X, class L, class R>
struct is_forest<binary_tree<X, L, R>> : std::true_type {};

template <class, class, bool>
class tree_core;

template <class X, class... Forest>
class tree_core<X, mlk::data::list::strict::list<Forest...>, true> : public def_type<X> {
    template <template <class> class FM>
    struct bind_base {
        template <class Y>
        struct inner_func
            : def_type<typename Y::template bind<FM>> {};
    };
public:
    typedef mlk::data::list::strict::list<Forest...> forest;

    template <class T>
    using eta = tree<T, mlk::data::list::strict::list<>>;

    template <template <class> class FM>
    using bind = 
        tree<
            typename FM<X>::type::type, 
            typename FM<X>::type::forest::template concat<
                typename forest::template map<bind_base<FM>::template inner_func>
            >
        >;
};

template <class, class>
struct tree_base;

template <class X, class... Forest>
struct tree_base<X, mlk::data::list::strict::list<Forest...>>
    : tree_core<X, mlk::data::list::strict::list<Forest...>, std::conjunction_v<is_forest<Forest>...>> {};

} // namespace details

template <class X, class Forest>
struct tree 
    : details::tree_base<X, Forest> {};

template <class X, class L, class R>
struct binary_tree : details::tree_base<X, mlk::data::list::strict::list<L, R>> {
    using left_node = L;
    using right_node = R;
};

template <class T>
using eta = tree<T, mlk::data::list::strict::list<>>;

} // namespace mlk::data::tree

#endif
