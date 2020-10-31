#include "../../test_libs.hpp"
#include <mlk/data/list/strict.hpp>

using namespace mlk::data::list::strict;

template <class L, class R>
struct plus
    : mlk::def_type<std::integral_constant<int, L::value + R::value>> {};

BOOST_AUTO_TEST_SUITE(mlk_data_list_tests)

BOOST_AUTO_TEST_CASE(mlk_data_list_test1)
{
    constexpr int val1 = 
        list<std::integral_constant<int, 1>, std::integral_constant<int, 2>>
            ::foldr<plus, std::integral_constant<int, 1>>::value;
    static_assert(val1 == 4);
    BOOST_CHECK_EQUAL(val1, 4);
}

BOOST_AUTO_TEST_SUITE_END()
