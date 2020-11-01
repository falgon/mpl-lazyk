#ifndef INCLUDED_MLK_DATA_LIST_STRICT_HPP
#define INCLUDED_MLK_DATA_LIST_STRICT_HPP

#include "lazy.hpp"
#include "../core.hpp"

namespace mlk::data::list::strict {

template <class... Xs>
class list : public show_type<list<Xs...>> {
    typedef mlk::data::list::lazy::list<Xs...> base_type;
public:
    template <class X>
    using cons = 
        typename eval<typename base_type::template cons<X>>::template transfer<list>;

    template <class RPack>
    using concat = 
        typename eval<typename base_type::template concat<RPack>>::template transfer<list>;

    using head =
        typename base_type::head::type;

    using tail =
        typename base_type::tail::type::template transfer<list>;

    template <std::size_t I>
    using at =
        eval<typename base_type::template at<I>>;

    template <template <class> class F>
    using map =
        typename base_type::template map<fn<F>>::type::template transfer<list>;

    template <template <class, class> class F, class Acc>
    using foldr =
        typename base_type::template foldr<fn<F>, Acc>::type;

    template <template <class, class> class F, class Acc>
    using foldr1 =
        typename base_type::template foldr1<fn<F>>::type;

    template <template <class, class> class F, class Acc>
    using foldl =
        typename base_type::template foldl<fn<F>, Acc>::type;

    template <template <class, class> class F>
    using foldl1 =
        typename base_type::template foldl1<fn<F>>::type;

    template <template <class...> class C>
    using transfer = mlk::data::transfer<C, list>;

    inline static constexpr std::size_t length = base_type::length;
};

} // namespace mlk::data::list::strict

namespace mlk::data::details {

template <template <class...> class C, class... Xs>
struct transfer_base<C, mlk::data::list::strict::list<Xs...>>
    : def_type<C<Xs...>> {};

} // namespace mlk::data::details

#endif
