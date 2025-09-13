#pragma once

#include <bits/arch/default_arch.hpp>
#include <cstddef>
#include <cstdint>

namespace std::arch {

struct x86_64_arch : public default_arch {
    static constexpr size_t PAGE_SIZE = 1 << 12;
    static constexpr size_t NUM_PAGES = 1 << 16;
    static constexpr size_t MAX_SEGMENTS = 1 << 14;

    // io-operations
    static ptrdiff_t stdout_write(const void* data, size_t count);
    static ptrdiff_t stderr_write(const void* data, size_t count);
    static ptrdiff_t stdin_read(void* buffer, size_t count);

    // filesystem
    static constexpr std::size_t MAX_PATH_LEN = 4096;
    static constexpr std::size_t MAX_FILENAME_LEN = 256;

    using dir_stream_type = struct {
        int fd;
        int pos;
        long len;
        uint8_t buffer[4096];
    };
    using dir_entry_type = struct {
        unsigned long inode;
        char name[MAX_FILENAME_LEN];
        char type;
    };

    static dir_stream_type fs_dir_open(const char* path);
    static void fs_dir_close(dir_stream_type& stream);
    static bool fs_dir_read(dir_stream_type& stream, dir_entry_type& entry);
    static void fs_dir_get_current(char* buffer);
    static void fs_dir_set_current(const char* path);
    static const char* fs_dir_entry_name(dir_entry_type& entry);

    static bool fs_path_is_absolute(const char* p);
    static bool fs_path_root_dir(const char* p, const char** rd, size_t* len);
    static bool fs_path_root_name(const char* p, const char** rn, size_t* len);
    static bool fs_path_filename(const char* p, const char** f, size_t* len);
    static void fs_path_readlink(const char* path, char* buffer, size_t size);

    // threads
    using thread_handle_type = struct {
        int tid;
        int* clear_tid;
        void* stack_base;
        size_t stack_size;
    };

    static thread_handle_type thread_create(void* (*start)(void*), void* arg);
    static void thread_join(thread_handle_type h, void** retval);
    static void thread_detach(thread_handle_type h);

    // synchronization
    inline static int atomic_load(int& ref) {
        return __atomic_load_n(&ref, __ATOMIC_SEQ_CST);
    }
    inline static void atomic_store(int& ref, int val) {
        __atomic_store_n(&ref, val, __ATOMIC_SEQ_CST);
    }
    inline static int atomic_exchange(int& ref, int val) {
        return __atomic_exchange_n(&ref, val, __ATOMIC_SEQ_CST);
    }
    inline static bool atomic_compare_exchange(int& ref, int& expected,
                                               int desired) {
        return __atomic_compare_exchange_n(&ref, &expected, desired, true,
                                           __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    }
    inline static int atomic_fetch_add(int& ref, int val) {
        return __atomic_fetch_add(&ref, val, __ATOMIC_SEQ_CST);
    }

    // process
    static int exec(const char* path, char* argv[]);

    // runtime
    [[noreturn]] static void exit(int exit_code);
};

} // namespace std::arch