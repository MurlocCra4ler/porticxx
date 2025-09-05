#pragma once

#include <bits/type_traits/helpers.hpp>
#include <cstddef>

namespace std {

/************************/
/* Forward declarations */
/************************/

template <class T, class U> struct is_same;

template <class T> struct remove_cv;

template <class T> struct add_const;

template <class T> struct add_lvalue_reference;

template <class T> struct add_rvalue_reference;

/***************************/
/* Primary type categories */
/***************************/

// is_void
template <class T>
struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};

template <class T> constexpr bool is_void_v = is_void<T>::value;

// is_integral
template <class T>
    struct is_integral
    : std::bool_constant < requires(T t, T* p, void (*f)(T)) {
    reinterpret_cast<T>(t); // Exclude class types
    f(0);                   // Exclude enumeration types
    p + t; // Exclude everything not yet excluded but integral types
} > {};

template <class T> constexpr bool is_integral_v = is_integral<T>::value;

// is_floating_point
template <class T>
struct is_floating_point
    : std::integral_constant<
          bool,
          std::is_same<float, typename std::remove_cv<T>::type>::value ||
              std::is_same<double, typename std::remove_cv<T>::type>::value ||
              std::is_same<long double,
                           typename std::remove_cv<T>::type>::value> {};

template <class T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;

// is_array
template <class T> struct is_array : std::false_type {};

template <class T> struct is_array<T[]> : std::true_type {};

template <class T, size_t N> struct is_array<T[N]> : std::true_type {};

template <class T> constexpr bool is_array_v = is_array<T>::value;

// is_enum
template <class T> using is_enum = bool_constant<__is_enum(T)>;

template <class T> constexpr bool is_enum_v = is_enum<T>::value;

template <class T> using is_union = bool_constant<__is_union(T)>;

template <class T> constexpr bool is_union_v = is_union<T>::value;

namespace impl_unary_type_traits {
template <class T> integral_constant<bool, !is_union_v<T>> test(int T::*);

template <class> false_type test(...);
} // namespace impl_unary_type_traits

template <class T>
struct is_class : decltype(impl_unary_type_traits::test<T>(nullptr)) {};

template <class T> constexpr bool is_class_v = is_class<T>::value;

// is_function
template <class> struct is_function : std::false_type {};

template <class Ret, class... Args>
struct is_function<Ret(Args...)> : std::true_type {};

template <class Ret, class... Args>
struct is_function<Ret(Args..., ...)> : std::true_type {};

template <class Ret, class... Args>
struct is_function<Ret(Args...) const> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) volatile> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const volatile> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) volatile> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const volatile> : std::true_type {};

template <class Ret, class... Args>
struct is_function<Ret(Args...)&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) volatile&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const volatile&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...)&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) volatile&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const volatile&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) &&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const&&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) volatile&&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const volatile&&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) &&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const&&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) volatile&&> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const volatile&&> : std::true_type {};

template <class Ret, class... Args>
struct is_function<Ret(Args...) noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) volatile noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const volatile noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) volatile noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const volatile noexcept> : std::true_type {
};
template <class Ret, class... Args>
struct is_function<Ret(Args...) & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) volatile & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const volatile & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) volatile & noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const volatile & noexcept>
    : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) volatile && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args...) const volatile && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) volatile && noexcept> : std::true_type {};
template <class Ret, class... Args>
struct is_function<Ret(Args..., ...) const volatile && noexcept>
    : std::true_type {};

/*****************************/
/* Composite type categories */
/*****************************/

// is_arithmetic
template <class T>
struct is_arithmetic
    : std::integral_constant<bool, std::is_integral<T>::value ||
                                       std::is_floating_point<T>::value> {};

template <class T> constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

// is_object
template <class T> using is_object = bool_constant<__is_object(T)>;

template <class T> constexpr bool is_object_v = is_object<T>::value;

/********************/
/* Type properties  */
/********************/

// is_unsigned
namespace impl::type_properties {
template <typename T, bool = std::is_arithmetic<T>::value>
struct is_unsigned : std::integral_constant<bool, T(0) < T(-1)> {};

template <typename T> struct is_unsigned<T, false> : std::false_type {};
} // namespace impl::type_properties

template <typename T>
struct is_unsigned : impl::type_properties::is_unsigned<T>::type {};

template <class T> constexpr bool is_unsigned_v = is_unsigned<T>::value;

/************************/
/* Supported operations */
/************************/

template <class T, class... Args>
using is_constructible = bool_constant<__is_constructible(T, Args...)>;

template <class T, class... Args>
inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

template <class T, class... Args>
using is_trivially_constructible =
    bool_constant<__is_trivially_constructible(T, Args...)>;

template <class T, class... Args>
using is_nothrow_constructible =
    bool_constant<__is_nothrow_constructible(T, Args...)>;

template <class T, class... Args>
inline constexpr bool is_nothrow_constructible_v =
    is_nothrow_constructible<T, Args...>::value;

// default constructible
template <class T>
struct is_default_constructible : std::is_constructible<T> {};

template <class T>
inline constexpr bool is_default_constructible_v =
    is_default_constructible<T>::value;

template <class T>
struct is_trivially_default_constructible : std::is_trivially_constructible<T> {
};

template <class T>
struct is_nothrow_default_constructible : std::is_nothrow_constructible<T> {};

// copy constructible
template <class T>
struct is_copy_constructible
    : std::is_constructible<T, typename std::add_lvalue_reference<
                                   typename std::add_const<T>::type>::type> {};

template <class T>
inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

template <class T>
struct is_trivially_copy_constructible
    : std::is_trivially_constructible<
          T, typename std::add_lvalue_reference<
                 typename std::add_const<T>::type>::type> {};

template <class T>
struct is_nothrow_copy_constructible
    : std::is_nothrow_constructible<
          T, typename std::add_lvalue_reference<
                 typename std::add_const<T>::type>::type> {};

// move constructible
template <class T>
struct is_move_constructible
    : std::is_constructible<T, typename std::add_rvalue_reference<T>::type> {};

template <class T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

template <class T>
struct is_trivially_move_constructible
    : std::is_trivially_constructible<
          T, typename std::add_rvalue_reference<T>::type> {};

template <class T>
struct is_nothrow_move_constructible
    : std::is_nothrow_constructible<
          T, typename std::add_rvalue_reference<T>::type> {};

template <class T>
inline constexpr bool is_nothrow_move_constructible_v =
    is_nothrow_move_constructible<T>::value;

// assignable
template <class T, class U>
using is_assignable = bool_constant<__is_assignable(T, U)>;

template <class T, class U>
using is_trivially_assignable = bool_constant<__is_trivially_assignable(T, U)>;

template <class T, class U>
using is_nothrow_assignable = bool_constant<__is_nothrow_assignable(T, U)>;

// move assignable
template <class T>
struct is_move_assignable
    : std::is_assignable<typename std::add_lvalue_reference<T>::type,
                         typename std::add_rvalue_reference<T>::type> {};

template <class T>
struct is_trivially_move_assignable
    : std::is_trivially_assignable<
          typename std::add_lvalue_reference<T>::type,
          typename std::add_rvalue_reference<T>::type> {};

template <class T>
struct is_nothrow_move_assignable
    : std::is_nothrow_assignable<typename std::add_lvalue_reference<T>::type,
                                 typename std::add_rvalue_reference<T>::type> {
};

template <class T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

} // namespace std