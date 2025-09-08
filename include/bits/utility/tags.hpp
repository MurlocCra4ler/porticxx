#pragma once

#include <cstddef>

namespace std {

// piecewise construction tag
struct piecewise_construct_t {
    explicit piecewise_construct_t() = default;
};

constexpr std::piecewise_construct_t piecewise_construct{};

// in-place construction tag
struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

template <class T> struct in_place_type_t {
    explicit in_place_type_t() = default;
};

template <class T> constexpr in_place_type_t<T> in_place_type{};

template <size_t I> struct in_place_index_t {
    explicit in_place_index_t() = default;
};

template <size_t I> constexpr in_place_index_t<I> in_place_index{};

} // namespace std