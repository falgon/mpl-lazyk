#ifndef INCLUDED_MLK_DATA_LIST_LAZY_HPP
#define INCLUDED_MLK_DATA_LIST_LAZY_HPP
#include "../../config.hpp"
#include "../core.hpp"

namespace mlk::data::list::lazy {

template <class... Xs>
class list : public show_type<list<Xs...>> {
    struct at_base {
        template <std::size_t, class...>
        struct result;
        template <std::size_t I, class T, class... Ts>
        struct result<I, T, Ts...> : result<I - 1, Ts...> {};
        template <class T, class... Ts>
        struct result<0, T, Ts...> : def_type<T> {};
    };

    template <class F>
    class map_base {
        static_assert(
            type_traits::is_func<F>::value, 
            "The type F must be satisfied type_traits::is_func constraint"
        );
    public:
        typedef
            list<typename F::template exec<Xs>::type...>
            type;
    };
    
    struct foldr_base {
        template <class, class, class...>
        struct result;
        
        template <class F, class Acc, class T, class... Ts>
        struct result<F, Acc, T, Ts...> 
            : F::template exec<T, typename result<F, Acc, Ts...>::type> {};

        template <class F, class Acc>
        struct result<F, Acc> 
            : def_type<Acc> {};
    };

    struct foldl_base {
        template <class, class, class...>
        struct result;

        template <class F, class Acc, class T, class... Ts>
        struct result<F, Acc, T, Ts...>
            : F::template exec<typename result<F, Acc, Ts...>::type, T> {};

        template <class F, class Acc>
        struct result<F, Acc>
            : def_type<Acc> {};
    };

    struct head_tail_base {
        template <class...>
        struct result 
            : std::pair<def_type<unit>, def_type<unit>> {};

        template <class T, class... Ts>
        struct result<T, Ts...> 
            : std::pair<def_type<T>, def_type<list<Ts...>>> {};
    };

    template <class Folder, class F>
    struct fold_from_list {
        template <class, class>
        struct result;

        template <class Acc, class... Ts>
        struct result<Acc, list<Ts...>> 
            : Folder::template result<F, Acc, Ts...> {};
    };

    template <class>
    struct concat_base;

    template <class... Ys>
    struct concat_base<list<Ys...>>
        : def_type<list<Xs..., Ys...>> {};
public:
    template <std::size_t I>
    using at = 
        typename at_base::template result<I, Xs...>;

    template <class F, MLK_REQUIRES(type_traits::is_func<F>) = nullptr>
    using map = 
        map_base<F>;

    using head = 
        typename head_tail_base::template result<Xs...>::first_type;

    using tail = 
        typename head_tail_base::template result<Xs...>::second_type;

    template <class F, class Acc>
    using foldr =
        typename foldr_base::template result<F, Acc, Xs...>;

    template <class F>
    using foldr1 = 
        typename fold_from_list<foldr_base, F>::template result<eval<head>, eval<tail>>;

    template <class F, class Acc>
    using foldl = 
        typename foldl_base::template result<F, Acc, Xs...>;

    template <class F>
    using foldl1 = 
        typename fold_from_list<foldl_base, F>::template result<eval<head>, eval<tail>>;

    template <class X>
    using cons = 
        def_type<list<X, Xs...>>;

    template <class RList>
    using concat = 
        concat_base<mlk::data::transfer<list, RList>>;

    inline static constexpr std::size_t length = sizeof...(Xs);

    template <template <class...> class C>
    using transfer =
        mlk::data::transfer<C, list>;
};

template <class F, class List>
using map = typename List::template map<F>;

template <class F, class Acc, class List>
using foldr = typename List::template foldr<F, Acc>;

template <class F, class Acc, class List>
using foldl = typename List::template foldl<F, Acc>;

template <class F, class List>
using foldr1 = typename List::template foldr1<F>;

template <class F, class List>
using foldl1 = typename List::template foldl1<F>;

} // namespace mlk::data::list::lazy

namespace mlk::data::details {

template <template <class...> class C, class... Xs>
struct transfer_base<C, mlk::data::list::lazy::list<Xs...>>
    : def_type<C<Xs...>> {};

} // namespace mlk::data::details

namespace mlk::details {

template <class... Ts>
struct eval_base<
    mlk::data::list::lazy::list<Ts...>,
    std::enable_if_t<std::conjunction_v<type_traits::is_metafunc<Ts>...>, std::nullptr_t>
> : def_type<
    mlk::data::list::lazy::list<typename Ts::type...>
> {};

} // namespace mlk::details

#endif
