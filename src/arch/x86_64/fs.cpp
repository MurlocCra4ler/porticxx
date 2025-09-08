#include <bits/arch/x86_64_arch.hpp>

#include "sys_call.hpp"

namespace std::arch {

void x86_64_arch::fs_readlink(const char* path, char* buffer, size_t size) {
    long len = sys_call3(SYS_readlink, reinterpret_cast<long>(path),
                         reinterpret_cast<long>(buffer), size - 1);
    if (len == -1) {
        // TODO: handle error
        buffer[0] = '\0';
        __builtin_trap();
    } else {
        buffer[len] = '\0';
    }
}

void x86_64_arch::fs_get_current(char* buffer) {
    x86_64_arch::fs_readlink("/proc/self/cwd", buffer,
                             x86_64_arch::MAX_PATH_LEN);
}

} // namespace std::arch
