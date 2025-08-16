#pragma once

#include <bits/memory/shared_pointer.hpp>

namespace std {

template<class T, class... Args>
shared_ptr<T> make_shared(Args&&... args);

}
