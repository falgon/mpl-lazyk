#include "../../test_libs.hpp"
#include <mlk/monad/identity/strict.hpp>
#include <mlk/type_traits.hpp>

using namespace mlk;
using namespace mlk::monad::identity::strict;

template <class X>
struct F1
    : def_type<eta<X>> {};

static_assert(type_traits::strict::is_monad<identity<int>>::value);
static_assert(type_traits::strict::is_monad<eta<int>>::value);
static_assert(
    std::is_same_v<
        run_identity<eta<int>::bind<F1>>,
        int
    >
);

BOOST_AUTO_TEST_SUITE(mlk_monad_identity_tests)

BOOST_AUTO_TEST_CASE(mlk_monad_identity_tests1)
{   
    BOOST_TEST(type_traits::strict::is_monad<identity<int>>::value);
    BOOST_TEST(type_traits::strict::is_monad<eta<int>>::value);

    constexpr bool test1 = std::is_same<
        run_identity<eta<int>::bind<F1>>,
        int
    >::value;
    BOOST_TEST(test1);
}

BOOST_AUTO_TEST_SUITE_END()
