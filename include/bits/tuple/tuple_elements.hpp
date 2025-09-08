#pragma once

#include <bits/tuple/tuple.hpp>

namespace std {

template <size_t I, class T> struct tuple_element;

template <size_t I, class Head, class... Tail>
struct tuple_element<I, tuple<Head, Tail...>>
    : tuple_element<I - 1, tuple<Tail...>> {};

template <class Head, class... Tail>
struct tuple_element<0, tuple<Head, Tail...>> {
    using type = Head;
};

template <size_t I, class Tuple>
using tuple_element_t = typename tuple_element<I, Tuple>::type;

} // namespace std
