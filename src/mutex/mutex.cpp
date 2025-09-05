#include <bits/arch/arch.hpp>
#include <bits/mutex/mutex.hpp>

namespace std {

void mutex::lock() {
    int my_ticket = arch::current_arch::atomic_fetch_add(next_, 1);

    while (arch::current_arch::atomic_load(serving_) != my_ticket) {
        arch::current_arch::thread_yield();
    }
}

bool mutex::try_lock() {
    int expected = arch::current_arch::atomic_load(next_);
    int curr = arch::current_arch::atomic_load(serving_);

    if (expected != curr)
        return false;
    if (arch::current_arch::atomic_compare_exchange(next_, expected,
                                                    expected + 1))
        return true;
    return false;
}

void mutex::unlock() { arch::current_arch::atomic_fetch_add(serving_, 1); }

} // namespace std