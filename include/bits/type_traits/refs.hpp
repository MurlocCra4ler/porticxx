#pragma once

namespace std {

/**************/
/* References */
/**************/

// remove_reference
template<class T> struct remove_reference { typedef T type; };
template<class T> struct remove_reference<T&> { typedef T type; };
template<class T> struct remove_reference<T&&> { typedef T type; };

template<class T>
using remove_reference_t = typename remove_reference<T>::type;

// add_lvalue_reference, add_rvalue_reference
namespace impl_refs {
    template<class T>
    struct type_identity { using type = T; };
 
    template<class T>
    auto try_add_lvalue_reference(int) -> type_identity<T&>;
    template<class T>
    auto try_add_lvalue_reference(...) -> type_identity<T>;
 
    template<class T>
    auto try_add_rvalue_reference(int) -> type_identity<T&&>;
    template<class T>
    auto try_add_rvalue_reference(...) -> type_identity<T>;
}
 
template<class T>
struct add_lvalue_reference
    : decltype(impl_refs::try_add_lvalue_reference<T>(0)) {};
 
template<class T>
struct add_rvalue_reference
    : decltype(impl_refs::try_add_rvalue_reference<T>(0)) {};

}
