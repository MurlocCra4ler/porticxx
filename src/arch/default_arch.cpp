#include <bits/arch/default_arch.hpp>
#include <bits/exception/exception.hpp>
#include <cstdint>
#include <cstring>
#include <exception>

#include "bits/arch/arch.hpp"

namespace std::arch {

const char* bad_arch_function::what() const noexcept {
    return "current_arch does not implement required function";
}

std::ptrdiff_t default_arch::stdout_write(const void*, std::size_t count) {
    return count;
}

std::ptrdiff_t default_arch::stderr_write(const void*, std::size_t count) {
    return count;
}

std::ptrdiff_t default_arch::stdin_read(void*, std::size_t count) { return 0; }

void* default_arch::memcpy(void* dst, const void* src, std::size_t n) {
    unsigned char* d_byte = static_cast<unsigned char*>(dst);
    const unsigned char* s_byte = static_cast<const unsigned char*>(src);

    size_t word_size = sizeof(unsigned long);
    while (n > 0 && (reinterpret_cast<uintptr_t>(d_byte) % word_size != 0 ||
                     reinterpret_cast<uintptr_t>(s_byte) % word_size != 0)) {
        *d_byte++ = *s_byte++;
        --n;
    }

    unsigned long* d_word = reinterpret_cast<unsigned long*>(d_byte);
    const unsigned long* s_word =
        reinterpret_cast<const unsigned long*>(s_byte);
    while (n >= word_size) {
        *d_word++ = *s_word++;
        n -= word_size;
    }

    d_byte = reinterpret_cast<unsigned char*>(d_word);
    s_byte = reinterpret_cast<const unsigned char*>(s_word);
    while (n--) {
        *d_byte++ = *s_byte++;
    }

    return dst;
}

void* default_arch::memset(void* dst, int value, std::size_t n) {
    unsigned char* byte_ptr = static_cast<unsigned char*>(dst);
    unsigned char byte = static_cast<unsigned char>(value);

    constexpr size_t word_size = sizeof(unsigned long);
    while (n > 0 && (reinterpret_cast<uintptr_t>(byte_ptr) % word_size != 0)) {
        *byte_ptr++ = byte;
        --n;
    }

    unsigned long word = 0;
    for (size_t i = 0; i < word_size; ++i) {
        word |= static_cast<unsigned long>(byte) << (i * 8);
    }

    unsigned long* word_ptr = reinterpret_cast<unsigned long*>(byte_ptr);
    while (n >= word_size) {
        *word_ptr++ = word;
        n -= word_size;
    }

    byte_ptr = reinterpret_cast<unsigned char*>(word_ptr);
    while (n--) {
        *byte_ptr++ = byte;
    }

    return dst;
}

int default_arch::atomic_load(int& ref) {
    int expected = ref;
    current_arch::atomic_compare_exchange(ref, expected, expected);
    return expected;
}

void default_arch::atomic_store(int& ref, int val) {
    int old = ref;
    while (!current_arch::atomic_compare_exchange(ref, old, val)) {
        old = ref;
    }
}

int default_arch::atomic_exchange(int& ref, int val) {
    int old = ref;
    while (!current_arch::atomic_compare_exchange(ref, old, val)) {
    }
    return old;
}

bool default_arch::atomic_compare_exchange(int& ref, int& expected,
                                           int desired) {
    bad_arch_function e;
    make_exception_ptr(e).abort();
    return false;
}

int default_arch::atomic_fetch_add(int& ref, int val) {
    int old = ref;
    while (true) {
        int desired = old + val;
        if (current_arch::atomic_compare_exchange(ref, old, desired)) {
            return old;
        }
    }
}

[[noreturn]] void default_arch::exit(int exit_code) { __builtin_trap(); }

[[noreturn]] void default_arch::abort() { __builtin_trap(); }

[[noreturn]] void
default_arch::terminate_on_exception(std::exception_ptr exception) {
    const char* msg = exception->what();
    std::size_t count = std::strlen(msg);
    stderr_write(msg, count);
    exit(-1);
}

} // namespace std::arch