#pragma once

namespace std {

template<class T> struct add_pointer;

template<bool B, class T, class F> struct conditional;

template<class T> struct is_array;
template<class T> struct is_function;

template<class T> struct remove_cv;
template<class T> struct remove_extent;
template<class T> struct remove_reference;

/*********************************/
/* Miscellaneous transformations */
/*********************************/

template<bool B, class T = void>
struct enable_if {};
 
template<class T>
struct enable_if<true, T> { typedef T type; };

template<bool B, class T = void>
using enable_if_t = typename enable_if<B,T>::type;

// decay
template<class T>
struct decay {
private:
    typedef typename std::remove_reference<T>::type U;

public:
    typedef typename std::conditional< 
        std::is_array<U>::value,
        typename std::add_pointer<typename std::remove_extent<U>::type>::type,
        typename std::conditional< 
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type
        >::type
    >::type type;
};

// conditional
template<bool B, class T, class F>
struct conditional { using type = T; };
 
template<class T, class F>
struct conditional<false, T, F> { using type = F; };

template< bool B, class T, class F >
using conditional_t = typename conditional<B,T,F>::type;

// void_t
template<typename... Ts>
struct make_void { typedef void type; };
 
template<typename... Ts>
using void_t = typename make_void<Ts...>::type;

}
