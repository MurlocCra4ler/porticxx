#pragma once

#include <type_traits>

namespace std {

template <class T> constexpr T&& forward(remove_reference_t<T>& t) noexcept;

template <class T> constexpr remove_reference_t<T>&& move(T&& t) noexcept;

template <class T>
constexpr auto move_if_noexcept(T& x) noexcept
    -> conditional_t<is_nothrow_move_constructible_v<T> ||
                         !is_copy_constructible_v<T>,
                     T&&, const T&> {
    if constexpr (is_nothrow_move_constructible_v<T> ||
                  !is_copy_constructible_v<T>) {
        return static_cast<T&&>(x);
    } else {
        return static_cast<const T&>(x);
    }
}

template<typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept {
    static_assert(false, "declval not allowed in an evaluated context");
}

} // namespace std