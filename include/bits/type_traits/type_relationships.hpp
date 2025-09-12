#pragma once

#include <bits/type_traits/helpers.hpp>
#include <bits/type_traits/unary_type_traits.hpp>

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

// is_convertible
namespace _impl_type_relationships {

template <typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept {
    static_assert(false, "declval not allowed in an evaluated context");
}

template <class T>
auto test_returnable(int) -> decltype(void(static_cast<T (*)()>(nullptr)),
                                      std::true_type{});
template <class> auto test_returnable(...) -> std::false_type;

template <class From, class To>
auto test_implicitly_convertible(int)
    -> decltype(void(declval<void (&)(To)>()(declval<From>())),
                std::true_type{});
template <class, class>
auto test_implicitly_convertible(...) -> std::false_type;
} // namespace _impl_type_relationships

template <class From, class To>
struct is_convertible
    : std::integral_constant<
          bool,
          (decltype(_impl_type_relationships::test_returnable<To>(0))::value &&
           decltype(_impl_type_relationships::test_implicitly_convertible<
                    From, To>(0))::value) ||
              (is_void<From>::value && is_void<To>::value)> {};

template <class From, class To>
constexpr bool is_convertible_v = is_convertible<From, To>::value;

} // namespace std
