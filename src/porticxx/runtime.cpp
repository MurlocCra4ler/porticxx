#include <bits/arch/arch.hpp>

extern "C" {

void* memcpy(void* dst, const void* src, size_t size) {
    return std::arch::current_arch::memcpy(dst, src, size);
}

void* memset(void* dst, int value, size_t size) {
    return std::arch::current_arch::memset(dst, value, size);
}

}
