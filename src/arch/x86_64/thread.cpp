#include <bits/arch/x86_64_arch.hpp>
#include <cstdint>
#include <linux/futex.h>
#include <iostream>
#include <sched.h>

#include "sys_call.hpp"

static constexpr long SYS_MMAP  = 9;
static constexpr long SYS_UNMAP = 11;
static constexpr long SYS_FUTEX = 202;

static const int PROT_READ  = 0x1;
static const int PROT_WRITE = 0x2;

static const int MAP_PRIVATE   = 0x02;
static const int MAP_ANONYMOUS = 0x20;
static const int MAP_STACK     = 0x20000;

static inline void* sys_mmap(size_t len) {
    long r = sys_call6(SYS_MMAP, reinterpret_cast<long>(nullptr), static_cast<long>(len), 
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0L);
    return (void*)r;
}

static inline int sys_munmap(void* addr, size_t len) {
    long ret = sys_call3(
        SYS_UNMAP,
        reinterpret_cast<long>(addr),
        static_cast<long>(len),
        0
    );
    return static_cast<int>(ret); // 0 = success, <0 = errno
}

static inline int sys_futex(int* uaddr, int futex_op, int val,
                            const struct timespec* timeout = nullptr,
                            int* uaddr2 = nullptr, int val3 = 0) {

    return static_cast<int>(sys_call6(
        SYS_FUTEX,
        reinterpret_cast<long>(uaddr),
        static_cast<long>(futex_op),
        static_cast<long>(val),
        reinterpret_cast<long>(timeout),
        reinterpret_cast<long>(uaddr2),
        static_cast<long>(val3)
    ));
}

int futex_wait(int* addr, int val) {
    return sys_futex(addr, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, val);
}

extern "C" long _sys_clone(long flags, long child_sp, long parent_tid, long child_tid, long);
extern "C" void _thread_entry();

extern "C" [[noreturn]] void _thread_kickoff(void* (*start)(void*), void* arg) {
    start(arg);
    std::arch::x86_64_arch::exit(0);
}

namespace std::arch {

x86_64_arch::thread_handle_type x86_64_arch::thread_create(void* (*start)(void*), void* arg) {
    // create stack
    constexpr size_t stack_size = 1ull << 20; 
    void* stack = sys_mmap(stack_size);

    uintptr_t* tos = reinterpret_cast<uintptr_t*>(reinterpret_cast<uint8_t*>(stack) + stack_size);
    tos = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(tos) & ~0xfull);
    tos -= 4;

    void** child_sp = reinterpret_cast<void**>(tos);
    child_sp[0] = reinterpret_cast<void*>(_thread_entry);
    child_sp[1] = arg;
    child_sp[2] = reinterpret_cast<void*>(start);
    child_sp[3] = reinterpret_cast<void*>(0xdeadbeef0badf00d);

    int* child_tid = static_cast<int*>(sys_mmap( sizeof(int)));
    *child_tid = 1;

    unsigned long flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
                          CLONE_SYSVSEM | CLONE_THREAD |
                          CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID;

    long rc = _sys_clone(
        static_cast<long>(flags),
        reinterpret_cast<long>(child_sp),
        0L,
        reinterpret_cast<long>(child_tid),
        0L);

    thread_handle_type h{};
    h.tid = static_cast<int>(rc);
    h.clear_tid  = child_tid;
    h.stack_base = stack;
    h.stack_size = stack_size;

    return h;
}

void x86_64_arch::thread_join(x86_64_arch::thread_handle_type h, void** retval) {
    int* addr = h.clear_tid;
    while (true) {
        int val = __atomic_load_n(addr, __ATOMIC_SEQ_CST);
        if (val == 0) break;
        futex_wait(addr, val);
    }

    sys_munmap(h.stack_base, h.stack_size);
    sys_munmap(h.clear_tid, sizeof(int));

    if (retval) {
        *retval = nullptr;
    }
}

void x86_64_arch::thread_detach(x86_64_arch::thread_handle_type h) {

}

}
