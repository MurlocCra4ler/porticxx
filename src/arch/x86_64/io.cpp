#include <bits/arch/x86_64_arch.hpp>

namespace std::arch {

ptrdiff_t x86_64_arch::stdout_write(const void* data, size_t count) {
    ptrdiff_t ret;

    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(1),
          "D"(1),
          "S"(data),
          "d"(count)
        : "rcx", "r11", "memory"
    );

    return (ret >= 0) ? ret : -1;
}

ptrdiff_t x86_64_arch::stdin_read(void* buffer, size_t count) {
    ptrdiff_t ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(0),
          "D"(0),
          "S"(buffer),
          "d"(count)
        : "rcx", "r11", "memory"
    );

    return (ret >= 0) ? ret : -1;
}

}