#pragma once

#include <bits/memory/shared_pointer.hpp>
#include <utility>

namespace std {

template<class T, class... Args>
shared_ptr<T> make_shared(Args&&... args) {
    using ctrl_t = impl_shared_ptr::ctrl_block_inplace<T>;
    auto* ctrl = new ctrl_t(std::forward<Args>(args)...);
    shared_ptr<T> sp;
    sp.ptr = ctrl->get_ptr();
    sp.ctrl = ctrl;
    return sp;
}

}
