#ifndef INCLUDED_MLK_TEXT_PARSEC_HPP
#define INCLUDED_MLK_TEXT_PARSEC_HPP
#include "../monad/maybe/strict.hpp"
#include "../monad/either/lazy.hpp"
#include "../monad/transformer/state_transformer/lazy.hpp"
#include "../data/val_pack.hpp"
#include "../data/list/strict.hpp"

namespace mlk::text::parsec {

template <class>
class parsec;

namespace details {
    
template <class X>
class curried_same {
    template <class Y>
    struct rst1 : std::is_same<X, Y> {};
public:
    typedef fn<rst1> result_func;
};

namespace et = mlk::monad::either::lazy;
namespace mm = mlk::monad::maybe::strict;
namespace mlt = mlk::monad::transformer::state_transformer::lazy;
namespace vp = mlk::data::val_pack;
namespace ll = mlk::data::list::strict;

template <class F>
using parse_state = 
    mlt::state_transformer<F, et::either<>>;

template <class X>
using eta =
    mlt::eta<X, et::either<>>;

using get =
    mlt::get<et::either<>>;

template <class X>
using put =
    mlt::put<X, et::either<>>;

template <class F>
using modify =
    mlt::modify<F, et::either<>>;

template <class SM, class S>
using run_parse_state =
    mlt::run_state_transformer<SM, S>;

template <class X>
using lift =
    mlt::lift<X>;

} // namespace details

template <class P, class S>
using run_parse =
    details::run_parse_state<typename P::un_parsec, S>;

using get = parsec<details::get>;

template <class X>
using eta = parsec<details::eta<X>>;

namespace details {

template <class F>
class to_parse_state_func {
    template <class A>
    struct rst1
        : mlk::def_type<typename F::template exec<A>::type::un_parsec> {};
public:
    typedef fn<rst1> result_func;
};

template <class A>
class anyone_result {
    template <class>
    struct rst1 
        : def_type<lift<et::right<typename A::head>>> {};
    typedef fn<rst1> rst1_fn;

    template <class T, class = std::nullptr_t>
    struct take_one
        : def_type<typename put<typename T::tail>::template bind<rst1_fn>> {};

    template <class T>
    struct take_one<T, std::enable_if_t<T::length == 0, std::nullptr_t>>
        : def_type<
            lift<et::left<vp::val_pack<MLK_PP_CHARS(23, "unexpected end of input")>>>
        > {};
public:
    typedef
        typename take_one<A>::type
        type;
};
typedef
    fn<anyone_result>
    anyone;


template <class F>
class satisfy_result {
    static_assert(type_traits::is_func<F>::value);

    template <class A, MLK_REQUIRES_DEFAULT_TP>
    struct rst2
        : def_type<
            lift<
                et::left<
                    vp::val_pack<MLK_PP_CHARS(35, "did not satisfy the given predicate")>
                >
            >
        > {};
    template <class A>
    struct rst2<A, MLK_REQUIRES(F::template exec<A>::type)>
        : def_type<eta<A>> {};
    typedef fn<rst2> rst2_fn;

    template <class A>
    struct rst1
        : def_type<typename anyone::exec<A>::type::template bind<rst2_fn>> {};
public:
    typedef fn<rst1> result_func;
};
template <class F>
using satisfy = typename satisfy_result<F>::result_func;

template <class>
struct mzero_result
    : def_type<et::left<vp::val_pack<MLK_PP_CHARS(19, "unknown parse error")>>> {};
typedef fn<mzero_result> mzero_base;

template <class ILPS, class IRPS>
class mplus_result {
    typedef ILPS LL;
    typedef IRPS RR;
    template <class L, class, class S, class = std::nullptr_t>
    struct cond2
        : def_type<run_parse<L, S>> {};

    template <class L, class R, class S>
    struct cond2<
        parsec<L>, parsec<R>, S,
        std::enable_if_t<run_parse<parsec<R>, S>::is_right, std::nullptr_t>
    > : def_type<run_parse<parsec<R>, S>> {};

    template <class L, class R, class S, class = std::nullptr_t>
    struct cond1 : cond2<L, eval_rec<R>, S> {};

    template <class L, class R, class S>
    struct cond1<
        parsec<L>, R, S,
        std::enable_if_t<run_parse<parsec<L>, S>::is_right, std::nullptr_t>
    > : def_type<run_parse<parsec<L>, S>> {};

    template <class S>
    struct rst1
        : cond1<eval_rec<LL>, RR, S> {};
public:
    typedef fn<rst1> result_func;
};

template <class A>
struct optional_base
    : def_type<mlk::text::parsec::eta<mm::just<A>>> {};
typedef fn<optional_base> optional_result;

template <class, class, class>
class between_consume;

template <class OF, class CF, class PF>
class between_consume<
    parsec<parse_state<OF>>, 
    parsec<parse_state<CF>>, 
    parsec<parse_state<PF>>
> {
    template <class>
    struct rst2 : def_type<parse_state<PF>> {};
    typedef fn<rst2> rst2_fn;

    template <class A>
    struct rst3 : def_type<typename parse_state<CF>::template eta<A>> {};
    typedef fn<rst3> rst3_fn;

    template <class>
    struct rst1
        : def_type<
            parsec<
                typename parse_state<OF>
                    ::template bind<rst2_fn>
                    ::template bind<rst3_fn>
            >
          > {};
public:
    typedef fn<rst1> result_func;
};

} // namespace details

using mzero =
    parsec<details::parse_state<details::mzero_base>>;

template <class Lhs, class Rhs>
using mplus =
    parsec<typename details::parse_state<typename details::mplus_result<Lhs, Rhs>::result_func>>;

// TODO: Use more generic function (functor fmap and alternative...)
template <class Alt>
using optional =
    mplus<
        typename Alt::template bind<details::optional_result>,
        eta<details::mm::nothing>
    >;

namespace details {

template <class P>
class many_consume {
    template <class>
    struct rst2;
    typedef fn<rst2> rst2_fn;

    template <class F>
    class rst3 {
        static_assert(type_traits::is_func<F>::value);
        template <class X>
        class recursive_func2 {
            template <class XS>
            struct inner_func
                : F::template exec<typename ll::list<X>::template concat<XS>> {};
        public:
            typedef fn<inner_func> result_func;
        };
        template <class X>
        struct recursive_func1
            : rst2_fn::template exec<typename recursive_func2<X>::result_func> {};
        
        template <class R>
        struct inner_func
            : def_type<
                mm::maybe_case<
                    mlk::text::parsec::eta<typename F::template exec<ll::list<>>::type>,
                    recursive_func1,
                    R
                >
              > {};
    public:
        typedef fn<inner_func> result_func;
    };

    template <class F>
    struct rst2
        : def_type<typename optional<P>::template bind<typename rst3<F>::result_func>> {};

    template <class>
     struct rst1
        : rst2<fn<def_type>> {};
public:
    typedef fn<rst1> result_func;
};

template <class P>
class many1_consume {
    template <class A>
    class rst2 {
        template <class Pack>
        struct inner_func
            : def_type<eta<typename ll::list<A>::template concat<Pack>>> {};
    public:
        typedef fn<inner_func> result_func;
    };
    template <class A>
    struct rst1
        : def_type<
            parsec<
                typename get
                    ::template bind<
                        typename to_parse_state_func<
                            typename many_consume<P>::result_func
                        >::result_func
                    >
                    ::template bind<typename rst2<A>::result_func>
            >
          > {};
public:
    typedef fn<rst1> result_func;
};

} // namespace details

template <class F>
class parsec<details::parse_state<F>> {
public:
    typedef details::parse_state<F> un_parsec;
private:
    template <class>
    struct anyone_consume_result
        : def_type<parsec<details::get::template bind<details::anyone>>> {};
    typedef fn<anyone_consume_result> anyone_consume;

    template <class G>
    class satisfy_consume_result {
        template <class>
        struct rst1
            : def_type<parsec<details::get::template bind<details::satisfy<G>>>> {};
    public:
        typedef fn<rst1> result_func;
    };

    template <class P>
    class many1_consume_result {
        template <class>
        struct rst1
            : def_type<
                typename P::template bind<typename details::many1_consume<P>::result_func>
              > {};
    public:
        typedef fn<rst1> result_func;
    };
public:
    template <class G>
    using bind =
        parsec<
            typename un_parsec::template bind<
                typename details::to_parse_state_func<G>::result_func
            >
        >;

    template <class X>
    using eta =
        parsec<typename un_parsec::template eta<X>>;

    using anyone =
        bind<anyone_consume>;

    template <class G>
    using satisfy =
        bind<typename satisfy_consume_result<G>::result_func>;

    template <class X>
    using one =
        bind<typename satisfy_consume_result<typename details::curried_same<X>::result_func>::result_func>;

    template <class Open, class Close, class P>
    using between =
        bind<typename details::between_consume<Open, Close, P>::result_func>;

    template <class P>
    using many =
        bind<typename details::many_consume<P>::result_func>;

    template <class P>
    using many1 =
        bind<typename many1_consume_result<P>::result_func>;
};

using anyone =
    parsec<details::get::bind<details::anyone>>;

template <class F>
using satisfy =
    parsec<details::get::template bind<typename details::satisfy<F>>>;

template <class X>
using one =
    parsec<
        details::get::bind<
            details::satisfy<typename details::curried_same<X>::result_func>
        >
    >;

template <class Open, class Close, class P>
using between =
    parsec<
        details::get::bind<
            typename details::to_parse_state_func<
                typename details::between_consume<Open, Close, P>::result_func
            >::result_func
        >
    >;

template <class P>
using many =
    parsec<
        details::get::template bind<
            typename details::to_parse_state_func<
                typename details::many_consume<P>::result_func
            >::result_func
        >
    >;

template <class P>
using many1 =
    typename P::template bind<typename details::many1_consume<P>::result_func>;

} // namespace mlk::text::parsec

#endif
