#pragma once

#include <cstddef>
#include <bits/exception/exception_ptr.hpp>

namespace std::arch {

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

    // syscalls
    [[noreturn]] static void exit(int exit_code);

    // excptions
    [[noreturn]] static void terminate_on_exception(std::exception_ptr exception);
};

}