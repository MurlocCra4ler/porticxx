#pragma once

#include <cstddef>
#include <cstdint>
#include <bits/exception/exception_ptr.hpp>

namespace std::arch {

template<typename Derived>
struct arch_base {
    // memory layout
    static constexpr std::size_t num_pages() {
        return Derived::NUM_PAGES;
    }

    static constexpr std::size_t page_size() {
        return Derived::PAGE_SIZE;
    }

    static constexpr std::size_t max_segments() {
        return Derived::MAX_SEGMENTS;
    }

    static constexpr uint8_t* heap() {
        static uint8_t heap[num_pages()*page_size()];
        return heap;
    }

    // memory operations
    static void* memcpy(void* dst, const void* src, size_t size) {
        return Derived::memcpy(dst, src, size);
    }

    static void* memset(void* dst, int value, size_t size) {
        return Derived::memset(dst, value, size);
    }

    // io-operations
    static std::ptrdiff_t stdout_write(const void* data, std::size_t count) {
        return Derived::stdout_write(data, count);
    }

    static std::ptrdiff_t stderr_write(const void* data, std::size_t count) {
        return Derived::stderr_write(data, count);
    }

    static std::ptrdiff_t stdin_read(void* buffer, std::size_t count) {
        return Derived::stdin_read(buffer, count);
    }

    // syscalls
    [[noreturn]] static void exit(int exit_code) {
        Derived::exit(exit_code);
    }

    // excptions
    [[noreturn]] static void terminate_on_exception(std::exception_ptr exception) {
        Derived::terminate_on_exception(exception);
    }
};

}