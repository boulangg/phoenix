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
