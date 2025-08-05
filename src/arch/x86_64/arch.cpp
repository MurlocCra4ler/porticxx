#include <arch.hpp>

namespace arch {

void exit(int code) {
    asm volatile(
        "mov %[code], %%rdi\n"
        "mov $60, %%rax\n"
        "syscall\n"
        :
        : [code] "r"(static_cast<long>(code))
        : "rax", "rdi"
    );
    __builtin_unreachable();
}

}
