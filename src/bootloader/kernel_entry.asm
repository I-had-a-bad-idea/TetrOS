section .text.start

[bits 32]
[extern main]
global _start

_start:
    call main ; call main inside kernel
    jmp $