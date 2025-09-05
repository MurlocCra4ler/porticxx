#pragma once

#include <bits/exception/exception_ptr.hpp>
#include <cstddef>

namespace std::arch {

struct bad_arch_function : public std::exception {
    const char* what() const noexcept override;
};

struct default_arch {
    static constexpr std::size_t PAGE_SIZE = 0;
    static constexpr std::size_t NUM_PAGES = 0;
    static constexpr std::size_t MAX_SEGMENTS = 0;

    // io-operations:
    static std::ptrdiff_t stdout_write(const void* data, std::size_t count);
    static std::ptrdiff_t stderr_write(const void* data, std::size_t count);
    static std::ptrdiff_t stdin_read(void* buffer, std::size_t count);

    // memory operations
    static void* memcpy(void* dst, const void* src, std::size_t n);
    static void* memset(void* dst, int value, std::size_t n);

    // threads
    static void thread_yield() { /* noop */ }

    // sync
    static int atomic_load(int& ref);
    static void atomic_store(int& ref, int val);
    static int atomic_exchange(int& ref, int val);
    static bool atomic_compare_exchange(int& ref, int& expected, int desired);
    static int atomic_fetch_add(int& ref, int val);

    // runtime
    [[noreturn]] static void exit(int exit_code);
    [[noreturn]] static void abort();

    // excptions
    [[noreturn]] static void
    terminate_on_exception(std::exception_ptr exception);
};

} // namespace std::arch