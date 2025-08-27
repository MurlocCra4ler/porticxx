#pragma once

#include <bits/exception/exception.hpp>
#include <bits/memory/smart_pointers.hpp>
#include <bits/type_traits/type_relationships.hpp>
#include <utility>

namespace std {

class exception_ptr {
public:
    exception_ptr();
    exception_ptr(const shared_ptr<exception>& ptr);
    
    explicit operator bool() const noexcept {
        return static_cast<bool>(m_ptr);
    }

    void abort() const;

    exception& operator*() const noexcept {
        return *m_ptr;
    }

    exception* operator->() const noexcept {
        return m_ptr.get();
    }

private:
    shared_ptr<exception> m_ptr;
};

template<class E>
constexpr exception_ptr make_exception_ptr(E e) noexcept {
    static_assert(is_base_of_v<exception, E>, "E must derive from std::exception");
    return exception_ptr(make_shared<exception>(std::move(e)));
}

}