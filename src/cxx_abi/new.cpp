#include <bits/memory/allocators.hpp>
#include <cstddef>

// TODO: add alignment
static std::allocator<uint8_t> allocator;

void* operator new(size_t size) { return allocator.allocate(size); }

void operator delete(void* ptr) noexcept {
    (void)ptr;
    __builtin_trap();
}

void operator delete(void* ptr, size_t size) noexcept {
    allocator.deallocate(static_cast<uint8_t*>(ptr), size);
}
