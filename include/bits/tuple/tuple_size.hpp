#pragma once

#include <bits/tuple/tuple.hpp>

namespace std {

template <class T> struct tuple_size;

template <class... Types>
struct tuple_size<tuple<Types...>>
    : integral_constant<size_t, sizeof...(Types)> {};

template <class T> constexpr size_t tuple_size_v = tuple_size<T>::value;

} // namespace std
