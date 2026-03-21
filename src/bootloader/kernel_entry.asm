section .text.start

[bits 32]
[extern main]
global _start

_start:
    ; Write to video memory to show that we are in protected mode
    mov eax, 0xb8000 ; video memory address
    mov word [eax], 0x0f41 ; white 'A' on black background

    call main ; call main inside kernel
    jmp $