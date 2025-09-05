#pragma once

#include <bits/memory/default_delete.hpp>

namespace std {

template <class T, class D = default_delete<T>> class unique_ptr {
public:
    using pointer = T*;
    using element_type = T;
    using deleter_type = D;

    // constructors
    explicit unique_ptr(pointer p) noexcept : m_ptr(p) {}

    // destructor
    constexpr ~unique_ptr() {
        if (m_ptr)
            m_deleter(m_ptr);
    }

    // observers
    constexpr pointer operator->() const noexcept { return m_ptr; }

private:
    pointer m_ptr = nullptr;
    deleter_type m_deleter;
};

} // namespace std
