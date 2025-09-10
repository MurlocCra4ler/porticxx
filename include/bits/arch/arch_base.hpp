#pragma once

#include <bits/exception/exception_ptr.hpp>
#include <cstddef>
#include <cstdint>

namespace std::arch {

template <typename Derived> struct arch_base {
    // memory layout
    static constexpr std::size_t num_pages() { return Derived::NUM_PAGES; }

    static constexpr std::size_t page_size() { return Derived::PAGE_SIZE; }

    static constexpr std::size_t max_segments() {
        return Derived::MAX_SEGMENTS;
    }

    static constexpr uint8_t* heap() {
        static uint8_t heap[num_pages() * page_size()];
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

    // filesystem
    using dir_stream_type = Derived::dir_stream_type;
    using dir_entry_type = Derived::dir_entry_type;

    static constexpr std::size_t max_path_len() {
        return Derived::MAX_PATH_LEN;
    }

    static dir_stream_type fs_open_dir(const char* path) {
        return Derived::fs_open_dir(path);
    }
    static void fs_close_dir(dir_stream_type stream) {
        return Derived::fs_close_dir(stream);
    }
    static bool fs_read_dir(dir_stream_type stream, dir_entry_type& entry) {
        return Derived::fs_read_dir(stream, entry);
    }
    static void fs_get_current(char* buffer) {
        return Derived::fs_get_current(buffer);
    };

    // threads
    using thread_handle_type = Derived::thread_handle_type;

    static thread_handle_type thread_create(void* (*start)(void*), void* arg) {
        return Derived::thread_create(start, arg);
    }

    static void thread_join(thread_handle_type h, void** retval) {
        return Derived::thread_join(h, retval);
    }

    static void thread_detach(thread_handle_type h) {
        return Derived::thread_detach(h);
    }

    static void thread_yield() { return Derived::thread_yield(); }

    // synchronization
    inline static int atomic_load(int& ref) {
        return Derived::atomic_load(ref);
    }

    inline static void atomic_store(int& ref, int val) {
        Derived::atomic_store(ref, val);
    }

    inline static int atomic_exchange(int& ref, int val) {
        Derived::atomic_exchange(ref, val);
    }

    inline static bool atomic_compare_exchange(int& ref, int& expected,
                                               int desired) {
        return Derived::atomic_compare_exchange(ref, expected, desired);
    }

    inline static int atomic_fetch_add(int& ref, int val) {
        return Derived::atomic_fetch_add(ref, val);
    }

    // runtime
    [[noreturn]] static void exit(int exit_code) { Derived::exit(exit_code); }

    [[noreturn]] static void abort() { Derived::abort(); }

    // excptions
    [[noreturn]] static void
    terminate_on_exception(std::exception_ptr exception) {
        Derived::terminate_on_exception(exception);
    }
};

} // namespace std::arch