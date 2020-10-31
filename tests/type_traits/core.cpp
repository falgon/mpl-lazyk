#include "../test_libs.hpp"
#include <mlk/type_traits.hpp>

BOOST_AUTO_TEST_SUITE(mlk_type_traits_tests)

using namespace mlk::type_traits;

template <class X>
struct F : mlk::def_type<X> {};

BOOST_AUTO_TEST_CASE(mlk_type_traits_is_func_test)
{
    typedef mlk::fn<F> ftype;
    static_assert(is_func<ftype>::value);
    BOOST_TEST(is_func<ftype>::value);
}

BOOST_AUTO_TEST_CASE(mlk_type_traits_is_metafunc_test)
{
    static_assert(is_metafunc<F<int>>::value);
    BOOST_TEST(is_metafunc<F<int>>::value);
}

BOOST_AUTO_TEST_SUITE_END()
