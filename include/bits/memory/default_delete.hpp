#pragma once

namespace std {

template<class T>
struct default_delete {
    constexpr default_delete() noexcept = default;
    template<class U>
    constexpr default_delete(const default_delete<U>&) noexcept;
    constexpr void operator()(T*) const;
};
 
template<class T>
struct default_delete<T[]> {
    constexpr default_delete() noexcept = default;
    template<class U>
    constexpr default_delete(const default_delete<U[]>&) noexcept;
    template<class U>
    constexpr void operator()(U* ptr) const;
};

}
