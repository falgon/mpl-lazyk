#ifndef INCLUDED_MLK_MONAD_TRANSFORMER_STATE_TRANSFORMER_LAZY_HPP
#define INCLUDED_MLK_MONAD_TRANSFORMER_STATE_TRANSFORMER_LAZY_HPP
#include "../../config.hpp"
#include "../../type_traits.hpp"

namespace mlk::monad::transformer::lazy::state_transformer {

namespace details {

template <class F, class InnerMonad>
class state_method_base {
    template <class S>
    struct rst1
        : def_type<typename InnerMonad::template eta<typename F::template exec<S>::type>> {};
public:
    typedef fn<rst1> result_func;
};

template <class X>
class eta_result {
    template <class S>
    struct rst1
        : def_type<std::pair<X, S>> {};
public:
    typedef fn<rst1> result_func;
};

class get_result {
    template <class S>
    struct result_f
        : def_type<std::pair<S, S>> {};
public:
    typedef fn<result_f> result_func;
};

template <class X>
class put_result {
    template <class>
    struct result_f
        : def_type<std::pair<unit, X>> {};
public:
    typedef fn<result_f> result_func;
};

template <class F>
class modify_result {
    static_assert(type_traits::is_func<F>::value);
    template <class S>
    struct result_f
        : def_type<std::pair<unit, typename F::template exec<S>::type>> {};
public:
    typedef fn<result_f> result_func;
};
    
template <class T, class = std::nullptr_t>
struct evaluate
    : def_type<T> {};

template <class T>
struct evaluate<T, MLK_REQUIRES(type_traits::is_metafunc<T>)>
    : evaluate<typename T::type> {};

} // namespace details

template <class F, class InnerMonad>
class state_transformer {
    static_assert(type_traits::is_func<F>::value);
    static_assert(std::disjunction_v<type_traits::is_monad<InnerMonad>, type_traits::is_monad<InnerMonad>>);

    template <class K>
    class bind_result {
        static_assert(type_traits::is_func<K>::value);
        template <class AS>
        class rst2 {
            typedef typename AS::first_type A;
            typedef typename AS::second_type S;
            typedef typename K::template exec<A>::type stm;
        public:
            typedef typename stm::template apply<S> type;
        };
        typedef fn<rst2> rst1_fn;

        template <class RST>
        struct rst1
            : def_type<
                typename details::evaluate<RST>::type::template bind<rst1_fn>
              > {};

        template <class S>
        struct result_f
            : rst1<exec<F, S>> {};
    public:
        typedef fn<result_f> result_func;
    };

    template <class X>
    class eta_result {
        template <class S>
        struct rst1
            : def_type<typename InnerMonad::template eta<std::pair<X, S>>> {};
        typedef fn<rst1> rst1_fn;

        template <class>
        struct result_f
            : def_type<state_transformer<rst1_fn, InnerMonad>> {};
    public:
        typedef fn<result_f> result_func;
    };

    class const_get {
        template <class S>
        struct rst1
            : def_type<typename InnerMonad::template eta<std::pair<S, S>>> {};
        typedef fn<rst1> rst1_fn;

        template <class>
        struct result_f
            : def_type<state_transformer<rst1_fn, InnerMonad>> {};
    public:
        typedef fn<result_f> result_func;
    };

    template <class X>
    class const_put {
        template <class>
        struct rst1
            : def_type<typename InnerMonad::template eta<std::pair<unit, X>>> {};
        typedef fn<rst1> rst1_fn;

        template <class>
        struct result_f
            : def_type<state_transformer<rst1_fn, InnerMonad>> {};
    public:
        typedef fn<result_f> result_func;
    };

    template <class G>
    class const_modify {
        static_assert(type_traits::is_func<F>::value);
        template <class S>
        struct rst1
            : def_type<
                typename InnerMonad::template eta<std::pair<unit, typename G::template exec<S>::type>>
              > {};
        typedef fn<rst1> rst1_fn;

        template <class>
        struct result_f
            : def_type<state_transformer<rst1_fn, InnerMonad>> {};
    public:
        typedef fn<result_f> result_func;
    };
public:
    template <class S>
    using apply = exec<F, S>;

    template <class StateFM>
    using bind =
        state_transformer<
            typename bind_result<StateFM>::result_func,
            InnerMonad
        >;

    template <class A>
    using eta =
        bind<typename eta_result<A>::result_func>;

    using get =
        bind<typename const_get::result_func>;

    template <class X>
    using put =
        bind<typename const_put<X>::result_func>;

    template <class G>
    using modify =
        bind<typename const_modify<G>::result_func>;
};

template <class F, class InnerMonad>
using state_method =
    state_transformer<
        typename details::state_method_base<F, InnerMonad>::result_func, 
        InnerMonad
    >;

template <class A, class InnerMonad>
using eta =
    state_method<typename details::eta_result<A>::result_func, InnerMonad>;

template <class InnerMonad>
using get =
    state_method<details::get_result::result_func, InnerMonad>;

template <class X, class InnerMonad>
using put =
    state_method<typename details::put_result<X>::result_func, InnerMonad>;

template <class F, class InnerMonad>
using modify =
    state_method<typename details::modify_result<F>::result_func, InnerMonad>;

namespace details {

template <class, class>
struct state_transformer_runner;

template <class F, class InnerMonad, class S>
struct state_transformer_runner<state_transformer<F, InnerMonad>, S> {
    typedef state_transformer<F, InnerMonad> base_type;
public:
    typedef
        typename evaluate<typename base_type::template apply<S>>::type
        type;
};

template <class M>
class lift_result {
    template <class S>
    struct F2 {
        template <class A>
        struct F3
            : def_type<typename M::template eta<std::pair<A, S>>> {};
        typedef fn<F3> result_func;
    };

    template <class S>
    struct F1
        : def_type<typename M::template bind<typename F2<S>::result_func>> {};
    typedef fn<F1> rst1;
public:
    typedef state_transformer<rst1, M> type;
};

} // namespace details

template <class StateT, class S>
using run_state_transformer = typename details::state_transformer_runner<StateT, S>::type;

template <class M>
using lift = typename details::lift_result<M>::type;

} // namespace mlk::monad::transformer::state_transformer
    
#endif
