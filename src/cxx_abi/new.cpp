#include <cstddef>

[[noreturn]] void unreachable_new_call() {
    __builtin_trap();
}

void* operator new(size_t size) {
    unreachable_new_call();
}

void operator delete(void* ptr) noexcept  {
	(void) ptr;
}

void operator delete(void* ptr, size_t size) noexcept  {
	(void) ptr;
	(void) size;
}
