#pragma once

#include <cstddef>

namespace std {

/***********/
/* Classes */
/***********/

template <class Ptr> struct pointer_traits {};

template <class T> struct pointer_traits<T*> {
    using pointer = T*;
    using element_type = T;
    using difference_type = ptrdiff_t;

    template <class U> using rebind = U*;

    // static constexpr pointer pointer_to(/* see description */ r) noexcept;
};

} // namespace std