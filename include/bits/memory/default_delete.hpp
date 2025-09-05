#pragma once

namespace std {

template <class T> struct default_delete {
    constexpr default_delete() noexcept = default;
    template <class U>
    constexpr default_delete(const default_delete<U>&) noexcept;

    constexpr void operator()(T* ptr) const noexcept {
        static_assert(sizeof(T) > 0, "can't delete incomplete type");
        delete ptr;
    }
};

template <class T> struct default_delete<T[]> {
    constexpr default_delete() noexcept = default;
    template <class U>
    constexpr default_delete(const default_delete<U[]>&) noexcept;
    template <class U> constexpr void operator()(U* ptr) const;
};

} // namespace std
