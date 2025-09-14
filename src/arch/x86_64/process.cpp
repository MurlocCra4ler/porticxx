#include <bits/arch/x86_64_arch.hpp>

#include <sys/wait.h>
#include <unistd.h>

#include "sys_call.hpp"

namespace std::arch {

int x86_64_arch::exec(const char* path, const char* argv[]) {
    long pid = sys_call(SYS_fork);

    if (pid < 0) {
        __builtin_trap();
    }

    if (pid == 0) {
        sys_call3(SYS_execve, reinterpret_cast<long>(path),
                  reinterpret_cast<long>(argv), 0);

        // exec failed
        arch::x86_64_arch::exit(127);
    }

    int status = 0;
    sys_call3(SYS_wait4, pid, reinterpret_cast<long>(&status), 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

} // namespace std::arch
