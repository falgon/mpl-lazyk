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

BOOST_AUTO_TEST_CASE(mlk_data_list_test2)
{
    typedef
        list<
            std::integral_constant<int, 1>,
            std::integral_constant<int, 2>,
            std::integral_constant<int, 3>
        >
        ltype;
    constexpr bool test2_1 =
        std::is_same_v<typename ltype::head, std::integral_constant<int, 1>>;
    constexpr bool test2_2 =
        std::is_same_v<
            typename ltype::tail,
            list<std::integral_constant<int, 2>, std::integral_constant<int, 3>>
        >;

    static_assert(test2_1);
    BOOST_TEST(test2_1);
    static_assert(test2_2);
    BOOST_TEST(test2_2);
}

BOOST_AUTO_TEST_CASE(mlk_data_list_test3)
{
    typedef
        list<
            std::integral_constant<int, 1>,
            std::integral_constant<int, 2>
        >
        ltype;
    typedef
        ltype::cons<std::integral_constant<int, 3>>
        result_type;

    constexpr bool test3 =
        std::is_same_v<
            result_type,
            list<
                std::integral_constant<int, 3>,
                std::integral_constant<int, 1>,
                std::integral_constant<int, 2>
            >
        >;
    static_assert(test3);
    BOOST_TEST(test3);
}

BOOST_AUTO_TEST_CASE(mlk_data_list_test4)
{
    typedef
        list<
            std::integral_constant<int, 1>,
            std::integral_constant<int, 2>
        >
        ltype;
    typedef
        ltype::concat<
            list<
                std::integral_constant<int, 3>,
                std::integral_constant<int, 4>
            >
        >
        result_type;

    constexpr bool test4 =
        std::is_same_v<
            result_type,
            list<
                std::integral_constant<int, 1>,
                std::integral_constant<int, 2>,
                std::integral_constant<int, 3>,
                std::integral_constant<int, 4>
            >
        >;
    static_assert(test4);
    BOOST_TEST(test4);
}

BOOST_AUTO_TEST_SUITE_END()
