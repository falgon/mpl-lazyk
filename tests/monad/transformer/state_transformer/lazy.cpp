#include "../../../test_libs.hpp"
#include <mlk/monad/transformer/state_transformer/lazy.hpp>
#include <mlk/monad/identity/lazy.hpp>
#include <mlk/monad/either/lazy.hpp>
#include <mlk/data/val_pack.hpp>
#include <sstream>

namespace st = mlk::monad::transformer::state_transformer::lazy;
namespace id = mlk::monad::identity::lazy;
namespace et = mlk::monad::either::lazy;

BOOST_AUTO_TEST_SUITE(mlk_monad_transformer_lazy_state_transformer_tests)

template <class>
struct F1;
typedef mlk::fn<F1> f1;

template <class>
struct F2;
typedef mlk::fn<F2> f2;

template <class>
class any_result;

typedef mlk::fn<any_result> any;

typedef
    st::eta<std::integral_constant<std::size_t, 42>, et::either<>>
        ::bind<f1>
        ::bind<f2>
    test1_expr;

typedef
    st::eta<std::integral_constant<std::size_t, 42>, et::either<>>
        ::bind<f2>
        ::bind<f1>
    test2_expr;

typedef
    st::get<et::either<>>
        ::bind<any>
        ::get
        ::bind<any>
        ::get
        ::bind<any>
    test3_expr;

template <class>
struct F1
    : mlk::def_type<
        st::lift<
            et::left<mlk::data::val_pack::val_pack<MLK_PP_CHARS(4, "hoge")>>
        >
      > {};

template <class>
struct F2
    : mlk::def_type<
        st::lift<
            et::right<std::integral_constant<std::size_t, 53>>
        >
      > {};

template <class A>
class any_result {
    template <class>
    struct result_func
        : mlk::def_type<st::lift<et::right<typename A::head>>> {};
    typedef mlk::fn<result_func> rst1;
public:
    typedef
        typename st::put<typename A::tail, et::either<>>::template bind<rst1>
        type;
};

BOOST_AUTO_TEST_CASE(mlk_monad_transformer_lazy_state_transformer_test1)
{
    typedef
        st::run_state_transformer<test1_expr, mlk::unit>
        test1;

    constexpr bool test1_val =
        std::is_same_v<
            test1,
            et::left<mlk::data::val_pack::val_pack<MLK_PP_CHARS(4, "hoge")>>
        >;
    static_assert(test1_val);
    BOOST_TEST(test1_val);

    std::ostringstream os;
    os << et::from_left<test1>();
    BOOST_CHECK_EQUAL(os.str(), "hoge");
}

BOOST_AUTO_TEST_CASE(mlk_monad_transformer_lazy_state_transformer_test2)
{
    typedef
        st::run_state_transformer<test2_expr, mlk::unit>
        test2;

    constexpr bool test2_val =
        std::is_same_v<
            test2,
            et::left<mlk::data::val_pack::val_pack<MLK_PP_CHARS(4, "hoge")>>
        >;
    static_assert(test2_val);
    BOOST_TEST(test2_val);

    std::ostringstream os;
    os << et::from_left<test2>();
    BOOST_CHECK_EQUAL(os.str(), "hoge");
}

BOOST_AUTO_TEST_CASE(mlk_monad_transformer_lazy_state_transformer_test3)
{
    typedef
        st::run_state_transformer<
            test3_expr, 
            mlk::data::val_pack::val_pack<MLK_PP_CHARS(3, "abc")>::to_list
        >
        test3;

    constexpr bool test3_val =
        std::is_same_v<
            typename et::from_right<test3>::first_type,
            std::integral_constant<char, 'c'>
        >;
    static_assert(test3_val);
    BOOST_TEST(test3_val);
}

BOOST_AUTO_TEST_SUITE_END()
