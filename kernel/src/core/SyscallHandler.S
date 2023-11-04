#include "Constant.h"

.section .text
.code64

.global enable_syscall
enable_syscall:
	push %rcx
	push %rdx
	push %rax
# set bit 0 (SCE) of EFER register for syscall extension
	mov $0xC0000080, %ecx
    rdmsr
    or $0x1, %eax
    wrmsr
# set bit 16 (FSGSBASE) of cr4 register for [rd|wr][fs|gs] instructions
	mov %cr4, %rax
	or $0x10, %rax
	mov %rax, %cr4
    pop %rax
    pop %rdx
    pop %rcx
    ret

.global load_syscall
load_syscall:
	push %rcx
	push %rdx
	mov %edi, %eax
	shr $32, %rdi
	mov %edi, %edx
	mov $0xC0000081, %ecx	// START
	wrmsr
	mov %esi, %eax
	shr $32, %rsi
	mov %esi, %edx
	mov $0xC0000082, %ecx	// LSTAR
	wrmsr
	pop %rdx
	mov %edx, %eax
	shr $32, %rdx
	mov $0xC0000083, %ecx	// CSTAR
	wrmsr
	pop %rax
	mov $0, %edx
	mov $0xC0000084, %ecx	// SFMASK
	wrmsr
	ret

.global syscall64_handler
syscall64_handler:
	push %r11
	mov %rsp, %r11
	mov $SYSCALL_STACK_TOP, %rsp
	push %r11
	push %rcx						// rcx contain user rip
	mov %r10, %rcx
	push %rax						// rax contains the number of the function (7th argument on the stack according to the System V ABI)
	call syscall64
	pop %rcx
	pop %rcx
	pop %rsp
	pop %r11
	sysretq
