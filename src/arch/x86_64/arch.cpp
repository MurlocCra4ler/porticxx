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

[[noreturn]] void abort_exception(std::exception_ptr err) {
    //const char* msg = err
    exit(1);
}

}
