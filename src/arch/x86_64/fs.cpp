#include <cstdint>
#include <cstring>
#include <fcntl.h>

#include <bits/arch/x86_64_arch.hpp>
#include <unistd.h>

#include "sys_call.hpp"

namespace std::arch {

x86_64_arch::dir_stream_type x86_64_arch::fs_dir_open(const char* path) {
    long fd = sys_call3(SYS_open, reinterpret_cast<long>(path),
                        O_RDONLY | O_DIRECTORY, 0);

    if (fd < 0)
        __builtin_trap();

    return {.fd = static_cast<int>(fd), .pos = 0, .len = 0};
}

bool x86_64_arch::fs_dir_read(dir_stream_type& stream, dir_entry_type& entry) {
    struct linux_dirent {
        unsigned long d_ino;
        unsigned long d_off;
        unsigned short d_reclen;
        char d_name[];
    };

    linux_dirent* d = reinterpret_cast<linux_dirent*>(stream.buffer);
    if (stream.len == 0) {
        stream.len =
            sys_call3(SYS_getdents, stream.fd, reinterpret_cast<long>(d),
                      sizeof(stream.buffer));
        if (stream.len < 0) {
            __builtin_trap();
        } else if (stream.len == 0) {
            return false;
        }
        stream.pos = 0;
    } else if (stream.pos == stream.len) {
        return false;
    }

    // advance current to right position
    uint8_t* cur = stream.buffer + stream.pos;
    linux_dirent* e = reinterpret_cast<linux_dirent*>(cur);

    entry.inode = e->d_ino;
    entry.type = *(cur + d->d_reclen - 1);
    strncpy(entry.name, e->d_name, sizeof(entry.name));
    entry.name[sizeof(entry.name) - 1] = '\0';

    stream.pos += e->d_reclen;
    return true;
}

void x86_64_arch::fs_dir_get_current(char* buffer) {
    x86_64_arch::fs_path_readlink("/proc/self/cwd", buffer,
                                  x86_64_arch::MAX_PATH_LEN);
}

void x86_64_arch::fs_dir_set_current(const char* path) {
    sys_call1(SYS_chdir, reinterpret_cast<long>(path));
}

const char* x86_64_arch::fs_dir_entry_name(x86_64_arch::dir_entry_type& entry) {
    return entry.name;
}

bool x86_64_arch::fs_path_is_absolute(const char* p) {
    return p && p[0] == '/';
}

bool x86_64_arch::fs_path_root_dir(const char* p, const char** rd,
                                   size_t* len) {
    if (!p || p[0] != '/') {
        if (rd)
            *rd = nullptr;
        if (len)
            *len = 0;
        return false;
    }

    if (rd)
        *rd = p;
    if (len)
        *len = 1;
    return true;
}

bool x86_64_arch::fs_path_root_name(const char* p, const char** rn,
                                    size_t* len) {
    if (rn)
        *rn = nullptr;
    if (len)
        *len = 0;
    return false;
}

bool x86_64_arch::fs_path_filename(const char* p, const char** f, size_t* len) {
    if (!p || !*p) {
        // empty path
        if (f)
            *f = nullptr;
        if (len)
            *len = 0;
        return false;
    }

    // find last '/'
    const char* last_slash = nullptr;
    for (const char* c = p; *c; ++c) {
        if (*c == '/')
            last_slash = c;
    }

    const char* start = last_slash ? last_slash + 1 : p;
    size_t filename_len = 0;
    for (const char* c = start; *c; ++c)
        ++filename_len;

    if (f)
        *f = start;
    if (len)
        *len = filename_len;

    return filename_len > 0;
}

void x86_64_arch::fs_path_readlink(const char* p, char* buffer, size_t size) {
    long ret = sys_call3(SYS_readlink, reinterpret_cast<long>(p),
                         reinterpret_cast<long>(buffer), size - 1);
    if (ret == -1) {
        // TODO: handle error
        buffer[0] = '\0';
        __builtin_trap();
    } else {
        buffer[ret] = '\0';
    }
}

} // namespace std::arch
