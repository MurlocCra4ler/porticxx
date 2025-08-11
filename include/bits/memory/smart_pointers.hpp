#pragma once

#include <cstddef>

namespace std {

/******************/
/* Smart pointers */
/******************/

template<class T>
class shared_ptr {
public:

    // observers
    explicit operator bool() const noexcept;
};

template<class T>
class unique_ptr {};

template<class T, class... Args>
constexpr unique_ptr<T> make_unique(Args&&... args);

template<class T>
constexpr unique_ptr<T> make_unique(size_t size);

template<class T, class... Args>
shared_ptr<T> make_shared(Args&&... args);

}