#include <arch/wasm32_stubs/syscall.hpp>

extern "C" void runtime_yield();

namespace wasm32_stubs {

enum class syscall_id : int {
    exit = 0,
    write = 1,
    read = 2,
};

struct syscall_descriptor {
    syscall_id id;
    int args[8];
} static descriptor;

extern "C" void* get_syscall_descriptor() {
    return &descriptor;
}

void dispatch_syscall() {
    runtime_yield();
}

void exit(int exit_code) {
    descriptor.id = syscall_id::exit;
    descriptor.args[0] = exit_code;
    dispatch_syscall();
    __builtin_unreachable();
}

}
