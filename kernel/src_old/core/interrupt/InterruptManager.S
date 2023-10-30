.text

.global generic_exception_handler
.global generic_interrupt_handler
.global readCR2

readCR2:
	mov %cr2, %rax
	ret

.macro pushaq
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rdi
	push %rsi
	push %r8
	push %r9
	push %r10
	push %r11
	push %r12
	push %r13
	push %r14
	push %r15
.endm

.macro popaq
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rsi
	pop %rdi
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax
.endm

.macro exception_handler num
	push %rbp
    mov %rsp, %rbp
    pushaq
    mov \num, %rdi
    call generic_exception_handler
	popaq
    pop %rbp
    iretq
.endm

.macro exception_handler_errorcode num
    push %rbp
    mov %rsp, %rbp
	pushaq
    mov \num, %rdi
    mov 8(%rbp), %rsi
    call generic_exception_handler
	popaq
    pop %rbp
    add $8, %rsp
    iretq
.endm

.macro interrupt_handler num
    push %rbp
    mov %rsp, %rbp
    push %rdi
    mov \num, %rdi
    call interrupt_handler
	pop %rdi
    pop %rbp
    iretq
.endm

interrupt_handler:
	pushaq
	call generic_interrupt_handler
	popaq
	ret

#define EXCEPTION_HANDLER(num) \
.globl EXC_##num##_HANDLER;\
EXC_##num##_HANDLER: \
    exception_handler $0x##num


#define EXCEPTION_HANDLER_ERRCODE(num) \
.globl EXC_##num##_HANDLER;\
EXC_##num##_HANDLER: \
    exception_handler_errorcode $0x##num


#define INTERRUPT_HANDLER(num) \
.globl INT_##num##_HANDLER;\
INT_##num##_HANDLER: \
    interrupt_handler $0x##num


EXCEPTION_HANDLER(00);
EXCEPTION_HANDLER(01);
EXCEPTION_HANDLER(02);
EXCEPTION_HANDLER(03);
EXCEPTION_HANDLER(04);
EXCEPTION_HANDLER(05);
EXCEPTION_HANDLER(06);
EXCEPTION_HANDLER(07);
EXCEPTION_HANDLER_ERRCODE(08);
EXCEPTION_HANDLER(09);
EXCEPTION_HANDLER_ERRCODE(0A);
EXCEPTION_HANDLER_ERRCODE(0B);
EXCEPTION_HANDLER_ERRCODE(0C);
EXCEPTION_HANDLER_ERRCODE(0D);
EXCEPTION_HANDLER_ERRCODE(0E);
EXCEPTION_HANDLER(0F);
EXCEPTION_HANDLER(10);
EXCEPTION_HANDLER_ERRCODE(11);
EXCEPTION_HANDLER(12);
EXCEPTION_HANDLER(13);
EXCEPTION_HANDLER(14);
EXCEPTION_HANDLER(15);
EXCEPTION_HANDLER(16);
EXCEPTION_HANDLER(17);
EXCEPTION_HANDLER(18);
EXCEPTION_HANDLER(19);
EXCEPTION_HANDLER(1A);
EXCEPTION_HANDLER(1B);
EXCEPTION_HANDLER(1C);
EXCEPTION_HANDLER(1D);
EXCEPTION_HANDLER_ERRCODE(1E);
EXCEPTION_HANDLER(1F);

INTERRUPT_HANDLER(00);
INTERRUPT_HANDLER(01);
INTERRUPT_HANDLER(02);
INTERRUPT_HANDLER(03);
INTERRUPT_HANDLER(04);
INTERRUPT_HANDLER(05);
INTERRUPT_HANDLER(06);
INTERRUPT_HANDLER(07);
INTERRUPT_HANDLER(08);
INTERRUPT_HANDLER(09);
INTERRUPT_HANDLER(0A);
INTERRUPT_HANDLER(0B);
INTERRUPT_HANDLER(0C);
INTERRUPT_HANDLER(0D);
INTERRUPT_HANDLER(0E);
INTERRUPT_HANDLER(0F);
