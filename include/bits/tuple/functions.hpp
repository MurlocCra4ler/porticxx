#pragma once

#include <bits/tuple/tuple.hpp>
#include <bits/tuple/tuple_elements.hpp>
#include <bits/tuple/tuple_size.hpp>
#include <bits/utility/functions.hpp>
#include <bits/utility/integer_sequence.hpp>
#include <cstddef>

namespace std {

template <std::size_t I, class... Ts> constexpr auto& get(tuple<Ts...>& t) {
    using leaf_type =
        typename _impl_tuple::tuple_impl<I, Ts...>::template tuple_leaf<
            I, typename std::tuple_element<I, tuple<Ts...>>::type>;
    return static_cast<leaf_type&>(t).value;
}

template <std::size_t I, class... Ts>
constexpr const auto& get(const tuple<Ts...>& t) {
    using leaf_type =
        typename _impl_tuple::tuple_impl<I, Ts...>::template tuple_leaf<
            I, typename std::tuple_element<I, tuple<Ts...>>::type>;
    return static_cast<const leaf_type&>(t).value;
}

template <std::size_t I, class... Ts> constexpr auto&& get(tuple<Ts...>&& t) {
    using leaf_type =
        typename _impl_tuple::tuple_impl<I, Ts...>::template tuple_leaf<
            I, typename std::tuple_element<I, tuple<Ts...>>::type>;
    return static_cast<leaf_type&&>(t).value;
}

template <std::size_t I, class... Ts>
constexpr const auto&& get(const tuple<Ts...>&& t) {
    using leaf_type =
        typename _impl_tuple::tuple_impl<I, Ts...>::template tuple_leaf<
            I, typename std::tuple_element<I, tuple<Ts...>>::type>;
    return static_cast<const leaf_type&&>(t).value;
}

template <class... Types>
constexpr tuple<Types&&...> forward_as_tuple(Types&&... args) noexcept {
    return tuple<Types&&...>(std::forward<Types>(args)...);
}

namespace _impl_tuple {

template <class F, class Tuple, size_t... Is>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, index_sequence<Is...>) {
    return std::forward<F>(f)(get<Is>(std::forward<Tuple>(t))...);
}

} // namespace _impl_tuple

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t) {
    constexpr size_t N = tuple_size_v<decay_t<Tuple>>;
    return _impl_tuple::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
                                   make_index_sequence<N>{});
}

} // namespace std