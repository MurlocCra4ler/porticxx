#pragma once

#include <cstddef>

namespace std {

template <class T, T... Ints> class integer_sequence {};

namespace impl_utility {
template <class T, T I, T N, T... integers>
struct make_integer_sequence_helper {
    using type = typename make_integer_sequence_helper<T, I + 1, N, integers...,
                                                       I>::type;
};

template <class T, T N, T... integers>
struct make_integer_sequence_helper<T, N, N, integers...> {
    using type = integer_sequence<T, integers...>;
};
} // namespace impl_utility

template <class T, T N>
using make_integer_sequence =
    impl_utility::make_integer_sequence_helper<T, 0, N>::type;

template <size_t N>
using make_index_sequence = std::make_integer_sequence<std::size_t, N>;

template <size_t... Ints>
using index_sequence = integer_sequence<size_t, Ints...>;

template <class... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

} // namespace std
