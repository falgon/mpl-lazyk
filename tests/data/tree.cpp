#include "../test_libs.hpp"
#include <mlk/data/tree.hpp>
#include <mlk/type_traits/monad.hpp>

using namespace mlk;
using namespace mlk::data::list::strict;
using namespace mlk::data::tree;

BOOST_AUTO_TEST_SUITE(mlk_data_tree_tests)

BOOST_AUTO_TEST_CASE(mlk_data_tree_test1)
{
    typedef 
        tree<int, list<data::tree::eta<double>, data::tree::eta<float>>>
        type;

    constexpr bool 
        test1_val1 = std::is_same_v<typename type::type, int>,
        test1_val2 = std::is_same_v<typename type::forest::at<0>::type, double>,
        test1_val3 = std::is_same_v<typename type::forest::at<1>::type, float>;
    
    static_assert(test1_val1);
    static_assert(test1_val2);
    static_assert(test1_val3);

    BOOST_TEST(test1_val1);
    BOOST_TEST(test1_val2);
    BOOST_TEST(test1_val3);
}

BOOST_AUTO_TEST_CASE(mlk_data_tree_test2)
{
    typedef
        binary_tree<int, data::tree::eta<double>, data::tree::eta<float>>
        type;

    constexpr bool
        test2_val1 = std::is_same_v<typename type::type, int>,
        test2_val2 = std::is_same_v<typename type::left_node::type, double>,
        test2_val3 = std::is_same_v<typename type::right_node::type, float>;

    static_assert(test2_val1);
    static_assert(test2_val2);
    static_assert(test2_val3);
}

BOOST_AUTO_TEST_SUITE_END()
