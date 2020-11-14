#ifndef INCLUDED_MLK_APPS_LAZYK_EVALUATE_HPP
#define INCLUDED_MLK_APPS_LAZYK_EVALUATE_HPP
#include "combinators.hpp"
#include "parse.hpp"
#include <mlk/data/list/strict.hpp>

namespace mlk::lazyk {
namespace details {

using mlk::def_type;
using mlk::data::list::strict::list;

template <std::size_t n>
struct i2e
    : def_type<A<suc, typename i2e<n - 1>::type>> {};
template <>
struct i2e<0>
    : def_type<zero> {};

template <class...>
struct s2e_base
    : def_type<nil> {};

template <class X, class... Xs>
struct s2e_base<list<X, Xs...>> 
    : def_type<
        A<
            A<cons, typename i2e<static_cast<std::size_t>(X::value)>::type>, 
            typename s2e_base<list<Xs...>>::type
        >
    > {};

template <class T>
using s2e = typename s2e_base<T>::type;

template <std::size_t, class, class>
struct substitution_base;

template <std::size_t v, class W, class E>
struct substitution_base<v, mlk::data::tree::tree<lambda_elem<W>, list<>>, E>
    : def_type<L<typename substitution_base<v + 1, W, E>::type>> {};

template <std::size_t v, class M, class N, class E>
struct substitution_base<v, mlk::data::tree::binary_tree<app_elem, M, N>, E>
    : def_type<A<typename substitution_base<v, M, E>::type, typename substitution_base<v, N, E>::type>> {};

template <std::size_t v, class N, class E>
struct substitution_base<v, mlk::data::tree::tree<dbi_elem<N>, list<>>, E> {
private:
    template <std::size_t, std::size_t, class>
    struct inner_func2;
    template <std::size_t t, std::size_t vv, class W>
    struct inner_func2<t, vv, mlk::data::tree::tree<lambda_elem<W>, list<>>>
        : def_type<L<typename inner_func2<t + 1, vv, W>::type>> {};
    template <std::size_t t, std::size_t vv, class M, class NN>
    struct inner_func2<t, vv, mlk::data::tree::binary_tree<app_elem, M, NN>>
        : def_type<A<typename inner_func2<t, vv, M>::type, typename inner_func2<t, vv, NN>::type>> {};
    template <std::size_t t, std::size_t vv, class NN>
    struct inner_func2<t, vv, mlk::data::tree::tree<dbi_elem<NN>, list<>>>
        : std::conditional<NN::value < t, V<NN::value>, V<NN::value + vv>> {};

    template <class, class, class>
    struct inner_func1;
    template <class B2, class B3>
    struct inner_func1<std::bool_constant<true>, B2, B3>
        : def_type<V<N::value>> {};
    template <class B1, class B3>
    struct inner_func1<B1, std::bool_constant<true>, B3>
        : inner_func2<0, v, E> {};
    template <class B1, class B2>
    struct inner_func1<B1, B2, std::bool_constant<true>> {
    private:
        inline static constexpr std::size_t val = N::value - 1;
    public:
        typedef V<val> type;
    };
    inline static constexpr bool
        lt = N::value < v,
        eq = N::value == v,
        gt = N::value > v;
public:
    typedef
        typename inner_func1<
            std::bool_constant<lt>, 
            std::bool_constant<eq>, 
            std::bool_constant<gt>
        >::type
        type;
};

template <std::size_t v, class N, class E>
using substitution = typename substitution_base<v, N, E>::type;

template <class E>
class eval_lazyk_base
    : public def_type<E> {};

template <class F, class E>
class eval_lazyk_base<mlk::data::tree::binary_tree<app_elem, F, E>> {
    template <class>
    struct step;
    template <class V>
    struct step<
        mlk::data::tree::binary_tree<
            app_elem, 
            mlk::data::tree::tree<lambda_elem<V>, list<>>,
            E
        >
    >
    : public eval_lazyk_base<substitution<0, V, typename eval_lazyk_base<E>::type>> {};
    template <class G>
    struct step<
        mlk::data::tree::binary_tree<
            app_elem,
            G,
            E
        >
    >
    : def_type<A<G, typename eval_lazyk_base<E>::type>> {};
public:
    typedef
        typename step<A<typename eval_lazyk_base<F>::type, E>>::type
        type;
};

template <class E>
class eval_lazyk_base<mlk::data::tree::tree<lambda_elem<E>, list<>>>
    : public def_type<L<typename eval_lazyk_base<E>::type>> {};

template <class T>
using eval_lazyk = typename eval_lazyk_base<T>::type;

template <class Expr>
struct e2s_base {
private:
    template <class, std::size_t x>
    struct inner_func2
        : def_type<std::integral_constant<char, x>> {};
    template <class XX, class N, std::size_t x>
    struct inner_func2<mlk::data::tree::binary_tree<app_elem, XX, N>, x>
        : inner_func2<N, x + 1> {};

    template <class>
    struct inner_func1
        : def_type<std::integral_constant<char, '\0'>> {};
    template <class N>
    struct inner_func1<
        mlk::data::tree::tree<
            lambda_elem<
                mlk::data::tree::tree<
                    lambda_elem<
                        mlk::data::tree::tree<dbi_elem<N>, list<>>
                    >,
                    list<>
                >
            >, 
            list<>
        >
    >
    : def_type<std::integral_constant<char, '\0'>> {};
    template <class E>
    struct inner_func1<
        mlk::data::tree::tree<
            lambda_elem<
                mlk::data::tree::tree<
                    lambda_elem<E>,
                    list<>
                >
            >, 
            list<>
        >
    >
    : inner_func2<E, 0> {};
public:
    typedef typename e2s_base<eval_lazyk<A<cdr, Expr>>>::type::template cons<
        typename inner_func1<eval_lazyk<A<car, Expr>>>::type
    >
    type;
};

template <>
struct e2s_base<nil>
    : def_type<list<>> {};

template <class Val, class Input>
struct e2s
    : e2s_base<eval_lazyk<A<typename Val::first_type, s2e<Input>>>> {};

} // namespace details

template <class Input>
struct eval {
    template <class Val>
    struct result_func
        : details::e2s<Val, Input> {};
};

} // namespace mlk::lazyk
#endif
