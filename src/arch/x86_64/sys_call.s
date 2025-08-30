.section .text
.globl _sys_clone
.type _sys_clone, @function
.align 16
_sys_clone:
    mov %rcx, %r10
    mov $56, %eax
    syscall
    ret

.section .text
.globl _thread_entry
.type _thread_entry, @function
.align 16
_thread_entry:
    pop %rsi
    pop %rdi
    call _thread_kickoff
