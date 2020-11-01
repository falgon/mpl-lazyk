#ifndef INCLUDED_MLK_DATA_CORE_HPP
#define INCLUDED_MLK_DATA_CORE_HPP

namespace mlk::data {
namespace details {

template <template <class...> class, class>
struct transfer_base;

} // namespace details

template <template <class...> class C, class TC>
using transfer = typename details::transfer_base<C, TC>::type;

} // namespace mlk::data

#endif
