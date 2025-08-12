#include <bits/exception/exception_ptr.hpp>

extern "C" [[noreturn]] void porticxx_abort_exception(std::exception_ptr err);

namespace std {

exception_ptr::exception_ptr() = default;
exception_ptr::exception_ptr(const shared_ptr<exception>& ptr) : m_ptr(ptr) {}

void exception_ptr::abort() const {
    porticxx_abort_exception(*this);
}

}
