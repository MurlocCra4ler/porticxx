#pragma once

#include <utility>

namespace std {

template <class T = void> struct equal_to {
    constexpr bool operator()(const T& lhs, const T& rhs) const {
        return lhs == rhs;
    }
};

template <> struct equal_to<void> {
    template <class T, class U>
    constexpr auto operator()(T&& t,
                              U&& u) const -> decltype(std::forward<T>(t) ==
                                                       std::forward<U>(u));

    using is_transparent = void;
};

} // namespace std
