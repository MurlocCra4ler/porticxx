#pragma once

#include <bits/memory/default_delete.hpp>

namespace std {

template<class T, class D = default_delete<T>>
class unique_ptr {
public:
    using pointer      = T*;
    using element_type = T;
    using deleter_type = D;
};

}
