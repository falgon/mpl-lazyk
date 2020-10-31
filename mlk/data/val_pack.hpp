#ifndef INCLUDED_MLK_DATA_VAL_PACK_HPP
#define INCLUDED_MLK_DATA_VAL_PACK_HPP
#include "list.hpp"

namespace mlk::data::val_pack {

template <auto... xs>
struct val_pack {
    friend std::ostream& operator<<(std::ostream& os, const val_pack&)
    {
        return (os.put(xs), ...), os;
    }
    inline static constexpr std::size_t length = sizeof...(xs);
    typedef 
        mlk::data::list::strict::list<std::integral_constant<decltype(xs), xs>...> to_list;
};

namespace details {

template <class>
struct to_val_pack_base;

template <class... Xs>
struct to_val_pack_base<mlk::data::list::strict::list<Xs...>>
    : def_type<val_pack<Xs::value...>> {};

template <class... Xs>
struct to_val_pack_base<mlk::data::list::lazy::list<Xs...>>
    : def_type<val_pack<Xs::value...>> {};

} // namespace details

template <class Pack>
using to_val_pack = typename details::to_val_pack_base<Pack>::type;

} // namespace mlk::data::val_pack

#endif
