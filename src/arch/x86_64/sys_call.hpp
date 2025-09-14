#pragma once

#include <sys/syscall.h>

static inline long sys_call6(long n, long a1, long a2, long a3, long a4,
                             long a5, long a6) {
    unsigned long ret;
    register long r10 asm("r10") = a4;
    register long r8 asm("r8") = a5;
    register long r9 asm("r9") = a6;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_call5(long n, long a1, long a2, long a3, long a4,
                             long a5) {
    unsigned long ret;
    register long r10 asm("r10") = a4;
    register long r8 asm("r8") = a5;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_call3(long n, long a1, long a2, long a3) {
    unsigned long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_call1(long n, long a) {
    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_call(long n) {
    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n)
                 : "rcx", "r11", "memory");
    return ret;
}
