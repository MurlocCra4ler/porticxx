#include <cstddef>

extern "C" {

void* __cxa_allocate_exception(size_t size) {
    return nullptr;
}

void __cxa_free_exception(void* ptr) {}

[[noreturn]]
void __cxa_throw(void* thrown_exception, void* tinfo, void (*dest)(void*)) {
    __builtin_trap();
}

int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle) {
    return 0;
}

}
