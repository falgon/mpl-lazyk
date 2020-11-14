#include "../../test_libs.hpp"
#include <mlk/monad/identity/lazy.hpp>
#include <mlk/type_traits.hpp>

using namespace mlk;
using namespace mlk::monad::identity::lazy;

template <class X>
struct F1
    : def_type<eta<X>> {};
typedef fn<F1> fn_type;

static_assert(type_traits::lazy::is_monad<identity<int>>::value);
static_assert(type_traits::lazy::is_monad<eta<int>>::value);
static_assert(
    std::is_same_v<
        run_identity<eta<int>::bind<fn_type>>,
        int
    >
);

BOOST_AUTO_TEST_SUITE(mlk_monad_identity_tests)

BOOST_AUTO_TEST_CASE(mlk_monad_identity_tests1)
{   
    BOOST_TEST(type_traits::lazy::is_monad<identity<int>>::value);
    BOOST_TEST(type_traits::lazy::is_monad<eta<int>>::value);
    constexpr bool test1 = std::is_same<
        run_identity<eta<int>::bind<fn_type>>,
        int
    >::value;
    BOOST_TEST(test1);
}

BOOST_AUTO_TEST_SUITE_END()
