#include "../test_libs.hpp"
#include <mlk/monad/transformer/state_transformer/lazy.hpp>
#include <mlk/text/parsec.hpp>
#include <mlk/monad/maybe.hpp>
#include <mlk/data/val_pack.hpp>
#include <mlk/type_traits.hpp>
#include <sstream>

using namespace mlk;
using namespace mlk::text;

namespace et = monad::either::lazy;
namespace mm = monad::maybe::strict;
namespace vl = data::val_pack;
namespace ll = data::list::strict;

BOOST_AUTO_TEST_SUITE(mlk_text_parsec_tests)

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test1)
{
    typedef
        parsec::run_parse<
            parsec::anyone::anyone::anyone,
            vl::val_pack<MLK_PP_CHARS(3, "abc")>::to_list
        >
        test1;

    constexpr char test1_val =
        et::from_right<test1>::first_type::value;
    static_assert(test1_val == 'c');
    BOOST_CHECK_EQUAL(test1_val, 'c');

    constexpr bool test1_stack =
        std::is_same_v<
            ll::list<>,
            typename et::from_right<test1>::second_type
        >;
    static_assert(test1_stack);
    BOOST_TEST(test1_stack);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test2)
{
    typedef
        parsec::run_parse<
            parsec::anyone::anyone::anyone::anyone,
            vl::val_pack<MLK_PP_CHARS(3, "abc")>::to_list
        >
        test2;

    constexpr bool test2_val =
        std::is_same_v<
            typename et::from_left<test2>,
            vl::val_pack<MLK_PP_CHARS(23, "unexpected end of input")>
        >;
    static_assert(test2_val);
    BOOST_TEST(test2_val);

    std::ostringstream os;
    os << et::from_left<test2>();
    BOOST_CHECK_EQUAL(os.str(), "unexpected end of input");
}

template <class> struct Predicate1;
typedef fn<Predicate1> pred1;
template <class V>
struct Predicate1   
    : mlk::type_traits::eq<V, std::integral_constant<char, 'a'>> {};

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test3)
{
    typedef 
        parsec::run_parse<
            parsec::satisfy<pred1>,
            vl::val_pack<MLK_PP_CHARS(1, "a")>::to_list
        >
        test3;

    constexpr bool test3_val =
        std::is_same_v<
            typename et::from_right<test3>::first_type,
            std::integral_constant<char, 'a'>
        >;
    static_assert(test3_val);
    BOOST_TEST(test3_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test4)
{
    typedef 
        parsec::run_parse<
            parsec::satisfy<pred1>,
            vl::val_pack<MLK_PP_CHARS(1, "b")>::to_list
        >
        test4;
    constexpr bool test4_val =
        std::is_same_v<
            typename et::from_left<test4>,
            vl::val_pack<MLK_PP_CHARS(35, "did not satisfy the given predicate")>
        >;
    static_assert(test4_val);
    BOOST_TEST(test4_val);

    std::ostringstream os;
    os << et::from_left<test4>();
    BOOST_CHECK_EQUAL(os.str(), "did not satisfy the given predicate");
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test5)
{
    typedef
        parsec::run_parse<
            parsec::one<std::integral_constant<char, 'a'>>,
            vl::val_pack<MLK_PP_CHARS(1, "a")>::to_list
        >
        test5;
    constexpr bool test5_val =
        std::is_same_v<
            typename et::from_right<test5>, 
            std::pair<std::integral_constant<char, 'a'>, ll::list<>>
        >;
    static_assert(test5_val);
    BOOST_TEST(test5_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test6)
{
    typedef
        parsec::run_parse<
            parsec::satisfy<pred1>
                ::anyone
                ::one<std::integral_constant<char, 'c'>>
                ::anyone,
            vl::val_pack<MLK_PP_CHARS(4, "abcd")>::to_list
        >
        test6;
    constexpr bool test6_val =
        std::is_same_v<
            typename et::from_right<test6>::first_type,
            std::integral_constant<char, 'd'>
        >;
    static_assert(test6_val);
    BOOST_TEST(test6_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test7_1)
{
    typedef
        parsec::run_parse<
            parsec::mplus<
                parsec::one<std::integral_constant<char, 'b'>>,
                parsec::satisfy<pred1>
            >,
            vl::val_pack<MLK_PP_CHARS(2, "ab")>::to_list
        >
        test7_1;
    constexpr bool test7_1_val =
        std::is_same_v<
            typename et::from_right<test7_1>::first_type,
            std::integral_constant<char, 'a'>
        >;
    static_assert(test7_1_val);
    BOOST_TEST(test7_1_val);
}

template <class>
struct Predicate2;

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test7_2)
{
    typedef
        parsec::run_parse<
            parsec::mplus<
                parsec::one<std::integral_constant<char, 'a'>>,
                parsec::satisfy<fn<Predicate2>>
            >,
            vl::val_pack<MLK_PP_CHARS(2, "ab")>::to_list
        >
        test7_2;
    constexpr bool test7_2_val =
        std::is_same_v<
            typename et::from_right<test7_2>::first_type,
            std::integral_constant<char, 'a'>
        >;
    static_assert(test7_2_val);
    BOOST_TEST(test7_2_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test7_3)
{
    struct incomplete;

    typedef
        parsec::mplus<
            parsec::one<std::integral_constant<char, 'a'>>,
            incomplete
        >
        test7_2_type;

    typedef
        parsec::run_parse<
            test7_2_type,
            vl::val_pack<MLK_PP_CHARS(2, "ab")>::to_list
        >
        test7_2;

    constexpr bool test7_2_val =
        std::is_same_v<
            typename et::from_right<test7_2>::first_type,
            std::integral_constant<char, 'a'>
        >;
    static_assert(test7_2_val);
    BOOST_TEST(test7_2_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test8_1)
{
    typedef
        parsec::run_parse<
            parsec::between<
                parsec::one<std::integral_constant<char, '('>>,
                parsec::one<std::integral_constant<char, ')'>>,
                parsec::satisfy<pred1>
            >,
            vl::val_pack<MLK_PP_CHARS(3, "(a)")>::to_list
        >
        test8_1;
    constexpr bool test8_1_val =
        std::is_same_v<
            typename et::from_right<test8_1>::first_type,
            std::integral_constant<char, 'a'>
        >;
    static_assert(test8_1_val);
    BOOST_TEST(test8_1_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test8_2)
{
	typedef
        parsec::run_parse<
            parsec::between<
                parsec::one<std::integral_constant<char, '('>>,
                parsec::one<std::integral_constant<char, ')'>>,
                parsec::satisfy<pred1>
            >::template between<
                parsec::one<std::integral_constant<char, '<'>>,
                parsec::one<std::integral_constant<char, '>'>>,
                parsec::one<std::integral_constant<char, 'b'>>
            >,
            vl::val_pack<MLK_PP_CHARS(6, "(a)<b>")>::to_list
        >
        test8_2;
    constexpr bool test8_2_val =
        std::is_same_v<
            typename et::from_right<test8_2>::first_type,
            std::integral_constant<char, 'b'>
        >;
    static_assert(test8_2_val);
    BOOST_TEST(test8_2_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test9_1)
{
    typedef
        parsec::run_parse<
            parsec::optional<parsec::one<std::integral_constant<char, 'a'>>>,
            vl::val_pack<MLK_PP_CHARS(1, "a")>::to_list
        >
        test9_1;
    constexpr bool test9_1_val =
        std::conjunction_v<
            std::bool_constant<et::from_right<test9_1>::first_type::is_just>,
            std::is_same<
                typename et::from_right<test9_1>::first_type,
                mm::just<std::integral_constant<char, 'a'>>
            >
        >;
    static_assert(test9_1_val);
    BOOST_TEST(test9_1_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test9_2)
{
    typedef
        parsec::run_parse<
            parsec::many<parsec::one<std::integral_constant<char, 'a'>>>,
            vl::val_pack<MLK_PP_CHARS(4, "aaab")>::to_list
        >
        test9_2;
    constexpr bool test9_2_val =
        std::is_same_v<
            typename et::from_right<test9_2>::second_type,
            vl::val_pack<MLK_PP_CHARS(1, "b")>::to_list
        >;
    static_assert(test9_2_val);
    BOOST_TEST(test9_2_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test9_3)
{
    typedef
        parsec::run_parse<
            parsec::many<parsec::one<std::integral_constant<char, 'a'>>>,
            vl::val_pack<MLK_PP_CHARS(1, "b")>::to_list
        >
        test9_3;
    constexpr bool test9_3_val =
        std::conjunction_v<
            std::is_same<
                typename et::from_right<test9_3>::first_type,
                ll::list<>
            >,
            std::is_same<
                typename et::from_right<test9_3>::second_type,
                vl::val_pack<MLK_PP_CHARS(1, "b")>::to_list
            >
        >;
    static_assert(test9_3_val);
    BOOST_TEST(test9_3_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test9_4)
{
    typedef
        parsec::run_parse<
            parsec::one<std::integral_constant<char, 'a'>>
                ::many<parsec::one<std::integral_constant<char, 'b'>>>,
            vl::val_pack<MLK_PP_CHARS(5, "abbbc")>::to_list
        >
        test9_4;
    constexpr bool test9_4_val =
        std::conjunction_v<
            std::is_same<
                typename et::from_right<test9_4>::first_type,
                vl::val_pack<MLK_PP_CHARS(3, "bbb")>::to_list
            >,
            std::is_same<
                typename et::from_right<test9_4>::second_type,
                vl::val_pack<MLK_PP_CHARS(1, "c")>::to_list
            >
        >;
    static_assert(test9_4_val);
    BOOST_TEST(test9_4_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test10_1)
{
    typedef
        parsec::run_parse<
            parsec::many1<parsec::one<std::integral_constant<char, 'a'>>>,
            vl::val_pack<MLK_PP_CHARS(3, "aaa")>::to_list
        >
        test10_1;
    constexpr bool test10_1_val =
        std::conjunction_v<
            std::is_same<
                typename et::from_right<test10_1>::first_type,
                vl::val_pack<MLK_PP_CHARS(3, "aaa")>::to_list
            >,
            std::is_same<
                typename et::from_right<test10_1>::second_type,
                ll::list<>
            >
        >;
    static_assert(test10_1_val);
    BOOST_TEST(test10_1_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test10_2)
{
    typedef
        parsec::run_parse<
            parsec::many1<parsec::one<std::integral_constant<char, 'a'>>>,
            ll::list<>
        >
        test10_2;
    constexpr bool test10_2_val =
        std::is_same_v<
            et::from_left<test10_2>,
            vl::val_pack<MLK_PP_CHARS(23, "unexpected end of input")>
        >;
    static_assert(test10_2_val);
    BOOST_TEST(test10_2_val);

    std::ostringstream os;
    os << et::from_left<test10_2>();
    BOOST_CHECK_EQUAL(os.str(), "unexpected end of input");
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test10_3)
{
    typedef
        parsec::run_parse<
            parsec::one<std::integral_constant<char, 'a'>>
                ::many1<parsec::one<std::integral_constant<char, 'b'>>>,
            vl::val_pack<MLK_PP_CHARS(5, "abbbc")>::to_list
        >
        test10_3;
    
    constexpr bool test10_3_val =
        std::conjunction_v<
            std::is_same<
                typename et::from_right<test10_3>::first_type,
                vl::val_pack<MLK_PP_CHARS(3, "bbb")>::to_list
            >,
            std::is_same<
                typename et::from_right<test10_3>::second_type,
                vl::val_pack<MLK_PP_CHARS(1, "c")>::to_list
            >
        >;
    static_assert(test10_3_val);
    BOOST_TEST(test10_3_val);
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test10_4)
{
    typedef
        parsec::run_parse<
            parsec::one<std::integral_constant<char, 'a'>>
                ::many1<parsec::mplus<parsec::one<std::integral_constant<char, 'b'>>, parsec::one<std::integral_constant<char, 'c'>>>>,
            vl::val_pack<MLK_PP_CHARS(5, "abbbc")>::to_list
        >
        test10_4;

    constexpr bool test10_4_val1 =
        std::is_same_v<
            typename et::from_right<test10_4>::first_type,
            vl::val_pack<MLK_PP_CHARS(4, "bbbc")>::to_list
        >;
    static_assert(test10_4_val1);
    BOOST_TEST(test10_4_val1);

    constexpr bool test10_4_val2 =
        std::is_same_v<
            typename et::from_right<test10_4>::second_type,
            ll::list<>
        >;
    static_assert(test10_4_val2);
    BOOST_TEST(test10_4_val2);            
}

BOOST_AUTO_TEST_CASE(mlk_text_parsec_test10_5)
{
    typedef
        parsec::run_parse<
            parsec::mplus<
                parsec::one<std::integral_constant<char, 'a'>>,
                parsec::one<std::integral_constant<char, 'b'>>
            >::eta<std::integral_constant<char, 'c'>>,
            vl::val_pack<MLK_PP_CHARS(1, "b")>::to_list
        >
        test10_5;
    
    constexpr char test10_5_val1 = et::from_right<test10_5>::first_type::value;
    static_assert(test10_5_val1 == 'c');
    BOOST_CHECK_EQUAL(test10_5_val1, 'c');

    constexpr bool test10_5_val2 =
        std::is_same_v<
            typename et::from_right<test10_5>::second_type,
            ll::list<>
        >;
    static_assert(test10_5_val2);
    BOOST_TEST(test10_5_val2);
}

BOOST_AUTO_TEST_SUITE_END()
