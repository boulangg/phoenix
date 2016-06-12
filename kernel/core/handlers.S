	.text
    .globl IT_32_handler
IT_32_handler:
    push %rax
    push %rdx
    push %rcx
    call PIT_handler
    pop %rcx
    pop %rdx
    pop %rax
    iretq


.globl IT_33_handler
IT_33_handler:
    push %rax
    push %rdx
    push %rcx
    call KEYBOARD_handler
    pop %rcx
    pop %rdx
    pop %rax
    iretq
