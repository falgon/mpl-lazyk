#ifndef INCLUDED_MLK_APP_LAZYK_PARSE_HPP
#define INCLUDED_MLK_APP_LAZYK_PARSE_HPP
#include <mlk/text/parsec.hpp>
#include <mlk/data/list/strict.hpp>
#include "combinators.hpp"

namespace mlk::lazyk {
namespace details {

using mlk::def_type;
using namespace mlk::text::parsec;

template <class>
struct eta_func
    : def_type<eta<S>> {};

using pS = 
    mplus<
        one<std::integral_constant<char, 'S'>>,
        one<std::integral_constant<char, 's'>>
    >::dbind<eta<S>>;

using pK = 
    mplus<
        one<std::integral_constant<char, 'K'>>,
        one<std::integral_constant<char, 'k'>>
    >::dbind<eta<K>>;

using pI =
    one<std::integral_constant<char, 'I'>>::dbind<eta<I>>;

using pU = 
    mplus<
        one<std::integral_constant<char, 'i'>>,
        one<std::integral_constant<char, '0'>>
    >::dbind<eta<U>>;

using pB = 
    one<std::integral_constant<char, '1'>>::dbind<eta<B>>;

template <class X, class Acc>
struct msum_func
    : def_type<mplus<X, Acc>> {};

struct pCb;
struct pAp;

using pTerm =
    mlk::data::list::strict::list<
        pS, pK, pI, pU, pB, pCb, pAp
    >::template foldr1<msum_func>;

template <class Pack>
class Afolder_result {
    template <class Acc, class X>
    struct F
        : def_type<A<Acc, X>> {};
public:
    typedef
        eta<typename Pack::template foldl1<F>>
        type;
};
typedef mlk::fn<Afolder_result> Afolder;

struct pCb
    : def_type<
        between<
            one<std::integral_constant<char, '('>>,
            one<std::integral_constant<char, ')'>>,
            many1<pTerm>::bind<Afolder>
        >
      > {};

struct pAp
    : def_type<
        mplus<
            one<std::integral_constant<char, '`'>>,
            one<std::integral_constant<char, '*'>>
        >::dbind<pTerm>::dbind<pTerm>
      > {};

using pExpr =
    mplus<
        between<
            one<std::integral_constant<char, '('>>,
            one<std::integral_constant<char, ')'>>,
            many1<pTerm>::bind<Afolder>
        >,
        many1<pTerm>::bind<Afolder>
    >;

} // namespace details

using parser =
    details::pExpr;

} // namespace mlk::lazyk

#endif
