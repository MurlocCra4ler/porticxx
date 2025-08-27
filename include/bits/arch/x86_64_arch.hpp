#pragma once

#include <cstddef>
#include <bits/arch/default_arch.hpp>

namespace std::arch {

struct x86_64_arch : public default_arch {
    static constexpr size_t PAGE_SIZE = 1 << 12;
    static constexpr size_t NUM_PAGES = 1 << 16;
    static constexpr size_t MAX_SEGMENTS = 1 << 14;

    // io-operations:
    static ptrdiff_t stdout_write(const void* data, size_t count);
    static ptrdiff_t stderr_write(const void* data, size_t count);
    static ptrdiff_t stdin_read(void* buffer, size_t count);

    // runtime
    [[noreturn]] static void exit(int exit_code);
};

}