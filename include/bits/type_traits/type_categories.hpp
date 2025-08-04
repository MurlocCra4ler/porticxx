#pragma once

#include <bits/type_traits/type_relations.hpp>
#include <bits/type_traits/cv_specifiers.hpp>

namespace std {

/***************************/
/* Primary type categories */
/***************************/

// is_void
template<class T>
struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};

template< class T >
constexpr bool is_void_v = is_void<T>::value;

// is_integral
template<class T>
struct is_integral : std::bool_constant<
    requires (T t, T* p, void (*f)(T)) {
        reinterpret_cast<T>(t); // Exclude class types
        f(0);                   // Exclude enumeration types
        p + t;                  // Exclude everything not yet excluded but integral types
    }> {};

template<class T>
constexpr bool is_integral_v = is_integral<T>::value;

// is_floating_point
template<class T>
struct is_floating_point
     : std::integral_constant<
         bool,
         std::is_same<float, typename std::remove_cv<T>::type>::value
         || std::is_same<double, typename std::remove_cv<T>::type>::value
         || std::is_same<long double, typename std::remove_cv<T>::type>::value> {};

template<class T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;

// is_enum
template<class T>
using is_enum = bool_constant<__is_enum(T)>;

template<class T>
constexpr bool is_enum_v = is_enum<T>::value;

/*****************************/
/* Composite type categories */
/*****************************/

// is_arithmetic
template<class T>
struct is_arithmetic : std::integral_constant<bool,
                                              std::is_integral<T>::value ||
                                              std::is_floating_point<T>::value> {};

template<class T>
constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

// is_object
template<class T>
using is_object = bool_constant<__is_object(T)>;

template<class T>
constexpr bool is_object_v = is_object<T>::value;

}