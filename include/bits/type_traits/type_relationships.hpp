#pragma once

#include <bits/type_traits/helpers.hpp>

namespace std {

template <class T> struct is_class;

/***********************/
/* Type relationships  */
/***********************/

// is_same
template <class T, class U> struct is_same : std::false_type {};

template <class T> struct is_same<T, T> : std::true_type {};

template <class T, class U> constexpr bool is_same_v = is_same<T, U>::value;

// is_base_of
namespace impl {
template <typename B> std::true_type test_ptr_conv(const volatile B*);
template <typename> std::false_type test_ptr_conv(const volatile void*);

template <typename B, typename D>
auto test_is_base_of(int)
    -> decltype(test_ptr_conv<B>(static_cast<D*>(nullptr)));
template <typename, typename>
auto test_is_base_of(...) -> std::true_type; // private or ambiguous base
} // namespace impl

template <typename Base, typename Derived>
struct is_base_of
    : std::integral_constant<
          bool, std::is_class<Base>::value &&
                    std::is_class<Derived>::
                        value&& decltype(impl::test_is_base_of<Base, Derived>(
                            0))::value> {};

template <class Base, class Derived>
constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

} // namespace std
