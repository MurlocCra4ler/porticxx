#include <cstddef>

extern "C" {

void* __cxa_allocate_exception(size_t size) {
    return nullptr;
}

void __cxa_free_exception(void* ptr) {}

void __cxa_pure_virtual() { 
    __builtin_trap();
}

[[noreturn]]
void __cxa_throw(void* thrown_exception, void* tinfo, void (*dest)(void*)) {
    __builtin_trap();
}

int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle) {
    return 0;
}

int __cxa_guard_acquire(long long* g) {
    if (*g) return 0;
    return 1;
}

void __cxa_guard_release(long long* g) {
    *g = 1;
}

void __cxa_guard_abort(long long*) {}

}
