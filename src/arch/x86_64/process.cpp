#include <cstdint>
#include <cstring>
#include <fcntl.h>

#include <bits/arch/x86_64_arch.hpp>
#include <unistd.h>

#include "sys_call.hpp"

namespace std::arch {

int x86_64_arch::exec(const char* path, char* argv[]) {
    return sys_call3(SYS_execve, reinterpret_cast<long>(path),
                     reinterpret_cast<long>(argv), 0);
}

} // namespace std::arch
