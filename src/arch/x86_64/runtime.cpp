#include <arch.hpp>

extern "C" void* __gxx_personality_v0 = nullptr;
extern "C" void* _Unwind_Resume = nullptr;
extern "C" void* __dso_handle = nullptr;

extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();

extern int main();

extern "C" void _start() {
    // C++-Konstruktoren aufrufen
    for (auto ctor = __init_array_start; ctor < __init_array_end; ++ctor) {
        (*ctor)();
    }

    int ret = main();
    arch::exit(ret);

    __builtin_unreachable();
}
