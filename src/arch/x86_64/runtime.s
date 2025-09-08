.section .text
.globl _start
.type _start, @function
.align 16
_start:
    xor %rbp, %rbp
    mov %rsp, %rdi
    sub $8, %rsp
    call runtime_start
    hlt
