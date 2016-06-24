
.text
#define TRAITANT_IT(nom, tic) \
.globl nom ;\
nom: \
    push %r15;\
    push %r14;\
    push %r13;\
    push %r12;\
    push %r11;\
    push %r10;\
    push %r9;\
    push %r8;\
    push %rbp ;\
    push %rdi ;\
    push %rsi ;\
    push %rdx ;\
    push %rcx ;\
    push %rbx ;\
    push %rax ;\
    call tic ;\
    pop %rax ;\
    pop %rbx ;\
    pop %rcx ;\
    pop %rdx ;\
    pop %rsi ;\
    pop %rdi ;\
    pop %rbp ;\
    pop %r8;\
    pop %r9;\
    pop %r10;\
    pop %r11;\
    pop %r12;\
    pop %r13;\
    pop %r14;\
    pop %r15;\
    iretq


// Registers saved when an interrupt occur (push on the new stack) :
// ss, cs, rsp, rip, rflags
// Registers that needs to be saved before entering tic handler
// rax, rbx, rcx, rdx, rsi, rdi, r8-15

// Registers to check later (float, status, ...)
// xmm*, mmx*, st*, status registers (other than rflags), ds, es, fs, gs

TRAITANT_IT(IT_32_handler, PIT_handler)
TRAITANT_IT(IT_33_handler, KEYBOARD_handler)

