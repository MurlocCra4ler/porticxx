#pragma once

#include <cstddef>

namespace std {

/************************/
/* Forward declarations */
/************************/

template <class T> struct is_function;

template <class T> struct is_array;

template <bool B, class T, class F> struct conditional;

/*******************************/
/* Const-volatility specifiers */
/*******************************/

template <class T> struct remove_cv {
    typedef T type;
};
template <class T> struct remove_cv<const T> {
    typedef T type;
};
template <class T> struct remove_cv<volatile T> {
    typedef T type;
};
template <class T> struct remove_cv<const volatile T> {
    typedef T type;
};

template <class T> using remove_cv_t = typename remove_cv<T>::type;

template <class T> struct remove_const {
    typedef T type;
};
template <class T> struct remove_const<const T> {
    typedef T type;
};

template <class T> struct remove_volatile {
    typedef T type;
};
template <class T> struct remove_volatile<volatile T> {
    typedef T type;
};

// add_cv
template <class T> struct add_cv {
    typedef const volatile T type;
};

template <class T> using add_cv_t = typename add_cv<T>::type;

// add_const
template <class T> struct add_const {
    typedef const T type;
};

template <class T> using add_const_t = typename add_const<T>::type;

// add_volatile
template <class T> struct add_volatile {
    typedef volatile T type;
};

template <class T> using add_volatile_t = typename add_volatile<T>::type;

/**************/
/* References */
/**************/

// remove_reference
template <class T> struct remove_reference {
    typedef T type;
};
template <class T> struct remove_reference<T&> {
    typedef T type;
};
template <class T> struct remove_reference<T&&> {
    typedef T type;
};

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

// add_lvalue_reference, add_rvalue_reference
namespace impl_refs {
template <class T> struct type_identity {
    using type = T;
};

template <class T> auto try_add_lvalue_reference(int) -> type_identity<T&>;
template <class T> auto try_add_lvalue_reference(...) -> type_identity<T>;

template <class T> auto try_add_rvalue_reference(int) -> type_identity<T&&>;
template <class T> auto try_add_rvalue_reference(...) -> type_identity<T>;
} // namespace impl_refs

template <class T>
struct add_lvalue_reference
    : decltype(impl_refs::try_add_lvalue_reference<T>(0)) {};

template <class T>
struct add_rvalue_reference
    : decltype(impl_refs::try_add_rvalue_reference<T>(0)) {};

/**********/
/* Arrays */
/**********/

template <class T> struct remove_extent {
    using type = T;
};

template <class T> struct remove_extent<T[]> {
    using type = T;
};

template <class T, std::size_t N> struct remove_extent<T[N]> {
    using type = T;
};

template <class T> using remove_extent_t = typename remove_extent<T>::type;

/************/
/* Pointers */
/************/

namespace impl_type_transformations {
template <class T> struct type_identity {
    using type = T;
}; // or use std::type_identity (since C++20)

template <class T>
auto try_add_pointer(int)
    -> type_identity<typename std::remove_reference<T>::type*>; // usual case

template <class T>
auto try_add_pointer(...)
    -> type_identity<T>; // unusual case (cannot form
                         // std::remove_reference<T>::type*)
} // namespace impl_type_transformations

template <class T>
struct add_pointer
    : decltype(impl_type_transformations::try_add_pointer<T>(0)) {};

/*************************/
/* Other transformations */
/*************************/

template <bool B, class T = void> struct enable_if {};

template <class T> struct enable_if<true, T> {
    typedef T type;
};

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

// decay
template <class T> struct decay {
private:
    typedef typename std::remove_reference<T>::type U;

public:
    typedef typename std::conditional<
        std::is_array<U>::value,
        typename std::add_pointer<typename std::remove_extent<U>::type>::type,
        typename std::conditional<
            std::is_function<U>::value, typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type>::type>::type type;
};

template <class T> using decay_t = typename decay<T>::type;

template <class T> struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <class T> using remove_cvref_t = remove_cvref<T>::type;

// conditional
template <bool B, class T, class F> struct conditional {
    using type = T;
};

template <class T, class F> struct conditional<false, T, F> {
    using type = F;
};

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

template <class F, class... ArgTypes> class invoke_result;

template <class F, class... ArgTypes>
using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

// void_t
template <typename... Ts> struct make_void {
    typedef void type;
};

template <typename... Ts> using void_t = typename make_void<Ts...>::type;

} // namespace std
