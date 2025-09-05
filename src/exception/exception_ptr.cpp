#include <bits/arch/arch.hpp>
#include <bits/exception/exception_ptr.hpp>

extern "C" [[noreturn]] void pcxx_terminate_on_exception(std::exception_ptr);

namespace std {

exception_ptr::exception_ptr() = default;
exception_ptr::exception_ptr(const shared_ptr<exception>& ptr) : m_ptr(ptr) {}

void exception_ptr::abort() const {
    arch::current_arch::terminate_on_exception(*this);
}

} // namespace std
