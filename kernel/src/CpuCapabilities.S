.section .text
.code64

.global enable_SSE
enable_SSE:
    mov %cr0, %rax
    and $0xFFFB, %ax # clear coprocessor emulation CR0.EM
    or $0x2, %rax     # set coprocessor monitoring  CR0.MP
    mov %rax, %cr0
    mov %cr4, %rax
    or $(3<<9), %rax # set CR4.OSFXSR and CR4.OSXMMEXCPT
    mov %rax, %cr4
    ret
