#include <bits/arch/arch.hpp>

extern "C" void* __gxx_personality_v0 = nullptr;
extern "C" void* _Unwind_Resume = nullptr;
extern "C" void* __dso_handle = nullptr;

extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();

extern "C" int main(int, char**, char**);

extern "C" void runtime_start(void* stack) {
    long* sp = (long*)stack;

    int argc = (int)sp[0];
    char** argv = (char**)&sp[1];
    char** envp = argv + argc + 1;

    for (auto ctor = __init_array_start; ctor < __init_array_end; ++ctor) {
        (*ctor)();
    }

    int ret = main(argc, argv, envp);

    std::arch::x86_64_arch::exit(ret);
}

namespace std::arch {

[[noreturn]] void x86_64_arch::exit(int code) {
    asm volatile("mov %[code], %%rdi\n"
                 "mov $60, %%rax\n"
                 "syscall\n"
                 :
                 : [code] "r"(static_cast<long>(code))
                 : "rax", "rdi");
    __builtin_unreachable();
}

} // namespace std::arch
