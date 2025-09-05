#include <bits/arch/arch.hpp>
#include <bits/thread/thread.hpp>
#include <exception>

namespace std {

thread::~thread() {
    if (joinable())
        terminate();
}

thread::thread(thread&& other) noexcept {
    m_handle = other.m_handle;
    other.m_handle.tid = 0;
}

thread& thread::operator=(thread&& other) noexcept {
    if (joinable())
        terminate();
    m_handle = other.m_handle;
    other.m_handle.tid = 0;
    return *this;
}

bool thread::joinable() const noexcept { return m_handle.tid > 0; }

void thread::join() { arch::current_arch::thread_join(m_handle, nullptr); }

} // namespace std