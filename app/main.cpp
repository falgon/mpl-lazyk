#include "evaluate.hpp"
#include <iostream>

#if !defined(INPUT) || !defined(INPUT_LEN)
#   define INPUT "Hello, world!"
#   define INPUT_LEN 13
#endif

#if !defined(LAZYK_CODE) || !defined(LAZYK_CODE_LEN)
#   define LAZYK_CODE "(SKK)I"
#   define LAZYK_CODE_LEN 6
#   define ENABLE_EXAMPLE_LAZYK_CODE
#endif

namespace et = mlk::monad::either::lazy;

typedef
    mlk::data::val_pack::val_pack<MLK_PP_CHARS(INPUT_LEN, INPUT)>::to_list
    input;

template <class T>
struct to_val_pack
    : mlk::def_type<mlk::data::val_pack::to_val_pack<T>> {};

template <class T>
struct eta
    : mlk::def_type<et::eta<T>> {};

template <template <class> class F>
using lift_either =
    mlk::composite<eta, F>;

template <class X>
struct remove_spaces
    : std::bool_constant<X::value != ' '> {};

int main()
{
    typedef
        mlk::text::parsec::run_parse<
            mlk::lazyk::parser,
            mlk::data::val_pack::val_pack<MLK_PP_CHARS(LAZYK_CODE_LEN, LAZYK_CODE)>
                ::to_list
                ::filter<remove_spaces>
        >
        parse_result;

    typedef
        parse_result::bind<
            mlk::fn<
                lift_either<mlk::lazyk::eval<input>::template result_func>::template result_func
            >
        >::bind<
            mlk::fn<
                lift_either<to_val_pack>::template result_func
            >
        >
        eval_result;

    typedef mlk::fn<mlk::def_type> ident;
    typedef
        et::either_case<ident, ident, eval_result>
        str_result;

    std::cout << str_result() << std::endl;
}
