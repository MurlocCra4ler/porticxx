#pragma once

#include <bits/type_traits/unary_type_traits.hpp>
#include <utility>

namespace std {

namespace impl_tuple {

template <std::size_t I, class T>
struct tuple_leaf {
    T value;

    constexpr tuple_leaf() = default;
    constexpr tuple_leaf(T const& v) : value(v) {}
    constexpr tuple_leaf(T&& v) : value(std::move(v)) {}
};

template <std::size_t I, class... Types>
struct tuple_impl;

template <size_t I, class Head, class... Tail>
struct tuple_impl<I, Head, Tail...> : tuple_leaf<I, Head>, tuple_impl<I+1, Tail...> {
    constexpr tuple_impl() = default;
};

template <std::size_t I>
struct tuple_impl<I> {};

template<typename T>
concept list_initializable = requires { T{}; };

}

template<class... Types>
class tuple : impl_tuple::tuple_impl<0, Types...> {
public:
    constexpr explicit(!(... && impl_tuple::list_initializable<Types>)) tuple()
    requires (... && is_default_constructible_v<Types>) = default;  
    
private:
    template<std::size_t I, class... Ts>
    friend constexpr auto& get(tuple<Ts...>& t);

    template<std::size_t I, class... Ts>
    friend constexpr const auto& get(const tuple<Ts...>& t);

    template<std::size_t I, class... Ts>
    friend constexpr auto&& get(tuple<Ts...>&& t);

    template<std::size_t I, class... Ts>
    friend constexpr const auto&& get(const tuple<Ts...>&& t);
};

}