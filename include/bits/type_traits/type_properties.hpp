#pragma once

#include <bits/type_traits/type_categories.hpp>

namespace std {

/********************/
/* Type properties  */
/********************/

// is_unsigned
namespace impl::type_properties {
    template<typename T, bool = std::is_arithmetic<T>::value>
    struct is_unsigned : std::integral_constant<bool, T(0) < T(-1)> {};

    template<typename T>
    struct is_unsigned<T, false> : std::false_type {};
}
 
template<typename T>
struct is_unsigned : impl::type_properties::is_unsigned<T>::type {};

template<class T>
constexpr bool is_unsigned_v = is_unsigned<T>::value;

}