#pragma once

#include <bits/arch/default_arch.hpp>
#include <cstddef>

namespace std::arch {

struct x86_64_arch : public default_arch {
    static constexpr size_t PAGE_SIZE = 1 << 12;
    static constexpr size_t NUM_PAGES = 1 << 16;
    static constexpr size_t MAX_SEGMENTS = 1 << 14;

    // io-operations
    static ptrdiff_t stdout_write(const void* data, size_t count);
    static ptrdiff_t stderr_write(const void* data, size_t count);
    static ptrdiff_t stdin_read(void* buffer, size_t count);

    // threads
    using thread_handle_type = struct {
        int tid;
        int* clear_tid;
        void* stack_base;
        size_t stack_size;
    };

    static thread_handle_type thread_create(void* (*start)(void*), void* arg);
    static void thread_join(thread_handle_type h, void** retval);
    static void thread_detach(thread_handle_type h);

    // synchronization
    inline static int atomic_load(int& ref) {
        return __atomic_load_n(&ref, __ATOMIC_SEQ_CST);
    }
    inline static void atomic_store(int& ref, int val) {
        __atomic_store_n(&ref, val, __ATOMIC_SEQ_CST);
    }
    inline static int atomic_exchange(int& ref, int val) {
        return __atomic_exchange_n(&ref, val, __ATOMIC_SEQ_CST);
    }
    inline static bool atomic_compare_exchange(int& ref, int& expected,
                                               int desired) {
        return __atomic_compare_exchange_n(&ref, &expected, desired, true,
                                           __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    }
    inline static int atomic_fetch_add(int& ref, int val) {
        return __atomic_fetch_add(&ref, val, __ATOMIC_SEQ_CST);
    }

    // runtime
    [[noreturn]] static void exit(int exit_code);
};

} // namespace std::arch