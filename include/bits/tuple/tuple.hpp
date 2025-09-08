#pragma once

#include <bits/type_traits/type_relationships.hpp>
#include <bits/utility/functions.hpp>

namespace std {

namespace _impl_tuple {

template <std::size_t I, class T> struct tuple_leaf {
    T value;

    constexpr tuple_leaf() = default;

    template <typename U>
    constexpr tuple_leaf(U&& v) : value(std::forward<U>(v)) {}
};

template <std::size_t I, class... Types> struct tuple_impl;

template <size_t I, class Head, class... Tail>
struct tuple_impl<I, Head, Tail...> : tuple_leaf<I, Head>,
                                      tuple_impl<I + 1, Tail...> {
    constexpr tuple_impl() = default;

    template <class H, class... T>
    constexpr tuple_impl(H&& h, T&&... t)
        : tuple_leaf<I, Head>(std::forward<H>(h)),
          tuple_impl<I + 1, Tail...>(std::forward<T>(t)...) {}
};

template <std::size_t I> struct tuple_impl<I> {};

template <typename T>
concept list_initializable = requires { T{}; };

} // namespace _impl_tuple

template <class... Types> class tuple : _impl_tuple::tuple_impl<0, Types...> {
public:
    constexpr explicit(!(... && _impl_tuple::list_initializable<Types>)) tuple()
        requires(... && is_default_constructible_v<Types>)
    = default;

    template <class... UTypes>
    constexpr explicit(!(... && std::is_convertible_v<UTypes, Types>))
        tuple(UTypes&&... args)
        : _impl_tuple::tuple_impl<0, Types...>(std::forward<UTypes>(args)...) {}

private:
    template <std::size_t I, class... Ts>
    friend constexpr auto& get(tuple<Ts...>& t);

    template <std::size_t I, class... Ts>
    friend constexpr const auto& get(const tuple<Ts...>& t);

    template <std::size_t I, class... Ts>
    friend constexpr auto&& get(tuple<Ts...>&& t);

    template <std::size_t I, class... Ts>
    friend constexpr const auto&& get(const tuple<Ts...>&& t);
};

} // namespace std