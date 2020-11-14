#include "../../test_libs.hpp"
#include <mlk/monad/either/strict.hpp>
#include <mlk/type_traits/monad/strict.hpp>

using namespace mlk;
namespace et = mlk::monad::either::strict;

template <class X>
struct F1 
    : def_type<
        et::left<std::integral_constant<std::size_t, X::value + 21>>
      > {};

template <class>
struct F2 
    : def_type<
        et::right<std::integral_constant<std::size_t, 53>>
      > {};

typedef
    et::either_case< // either leftf rightf e
        def_type,
        def_type,
        et::eta<std::integral_constant<std::size_t, 21>> // return 21
        ::template bind<F1> // >>= F1
        ::template bind<F2> // >>= F2
    >
    test1;

static_assert(
    std::is_same_v<
        et::either_case< // either leftf rightf e
            def_type,
            def_type,
            et::eta<std::integral_constant<std::size_t, 21>> // return 21
                ::template bind<F1> // >>= F1
                ::template bind<F2> // >>= F2
        >,
        std::integral_constant<std::size_t, 42>
    >
);

static_assert(
    mlk::type_traits::strict::is_monad<et::either<>>::value
);

BOOST_AUTO_TEST_SUITE(mlk_monad_either_tests)

BOOST_AUTO_TEST_CASE(mlk_monad_either_test1)
{
    BOOST_TEST(mlk::type_traits::strict::is_monad<et::either<>>::value);
    BOOST_CHECK_EQUAL(test1::value, 42);
}

BOOST_AUTO_TEST_SUITE_END()
