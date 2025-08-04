#pragma once

#include <bits/type_traits/helpers.hpp>
#include <bits/type_traits/refs.hpp>
#include <bits/type_traits/cv_specifiers.hpp>

namespace std {

/************************/
/* Supported operations */
/************************/

template< class T, class... Args >
using is_constructible = bool_constant<__is_constructible(T, Args...)>;

template< class T, class... Args >
using is_trivially_constructible = bool_constant<__is_trivially_constructible(T, Args...)>;

template< class T, class... Args >
using is_nothrow_constructible = bool_constant<__is_nothrow_constructible(T, Args...)>;

// default constructible
template<class T>
struct is_default_constructible : std::is_constructible<T> {};
 
template<class T>
struct is_trivially_default_constructible : std::is_trivially_constructible<T> {};
 
template<class T>
struct is_nothrow_default_constructible : std::is_nothrow_constructible<T> {};

// copy constructible
template<class T>
struct is_copy_constructible :
    std::is_constructible<T, typename std::add_lvalue_reference<
        typename std::add_const<T>::type>::type> {};
 
template<class T>
struct is_trivially_copy_constructible :
    std::is_trivially_constructible<T, typename std::add_lvalue_reference<
        typename std::add_const<T>::type>::type> {};
 
template<class T>
struct is_nothrow_copy_constructible :
    std::is_nothrow_constructible<T, typename std::add_lvalue_reference<
        typename std::add_const<T>::type>::type> {};

// move constructible
template<class T>
struct is_move_constructible :
    std::is_constructible<T, typename std::add_rvalue_reference<T>::type> {};
 
template<class T>
struct is_trivially_move_constructible :
    std::is_trivially_constructible<T, typename std::add_rvalue_reference<T>::type> {};
 
template<class T>
struct is_nothrow_move_constructible :
    std::is_nothrow_constructible<T, typename std::add_rvalue_reference<T>::type> {};

template<class T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

// assignable
template<class T, class U>
using is_assignable = bool_constant<__is_assignable(T, U)>;
 
template<class T, class U>
using is_trivially_assignable = bool_constant<__is_trivially_assignable(T, U)>;
 
template<class T, class U>
using is_nothrow_assignable = bool_constant<__is_nothrow_assignable(T, U)>;

// move assignable
template<class T>
struct is_move_assignable
    : std::is_assignable<typename std::add_lvalue_reference<T>::type,
                         typename std::add_rvalue_reference<T>::type> {};
 
template<class T>
struct is_trivially_move_assignable
    : std::is_trivially_assignable<typename std::add_lvalue_reference<T>::type,
                                   typename std::add_rvalue_reference<T>::type> {};
 
template<class T>
struct is_nothrow_move_assignable
    : std::is_nothrow_assignable<typename std::add_lvalue_reference<T>::type,
                                 typename std::add_rvalue_reference<T>::type> {};

template<class T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

}
