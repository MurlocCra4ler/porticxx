#pragma once

namespace std {

/*******************************/
/* Const-volatility specifiers */
/*******************************/

template<class T> struct remove_cv { typedef T type; };
template<class T> struct remove_cv<const T> { typedef T type; };
template<class T> struct remove_cv<volatile T> { typedef T type; };
template<class T> struct remove_cv<const volatile T> { typedef T type; };

template< class T >
using remove_cv_t = typename remove_cv<T>::type;
 
template<class T> struct remove_const { typedef T type; };
template<class T> struct remove_const<const T> { typedef T type; };
 
template<class T> struct remove_volatile { typedef T type; };
template<class T> struct remove_volatile<volatile T> { typedef T type; };

// add_cv
template<class T> struct add_cv { typedef const volatile T type; };

template<class T>
using add_cv_t = typename add_cv<T>::type;
 
// add_const
template<class T> struct add_const { typedef const T type; };

template<class T>
using add_const_t = typename add_const<T>::type;
 
// add_volatile
template<class T> struct add_volatile { typedef volatile T type; };

template<class T>
using add_volatile_t = typename add_volatile<T>::type;

}
