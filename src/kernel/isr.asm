[bits 32]
extern isr_handler
section .text


%macro ISR_NOERR 1
global isr_%1:
isr_%1:
    push 0              ; push fake error code
    push %1             ; push interrupt number
    jmp isr_common
%endmacro


%macro ISR_ERR 1
global isr_%1:
isr_%1:
                        ; cpu already pushes an error code
    push %1             ; push interrupt number
    jmp isr_common
%endmacro

%include "src/kernel/isr_gen.inc"

isr_common:
    pusha                    ; save general registers

    xor eax, eax             ; save old data segment
    mov ax, ds
    push eax                 ; push ds
    
    mov ax, 0x10             ; kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                 ; pass pointer to struct registers (stack)
    call isr_handler
    add esp, 4

    pop eax                  ; restore old segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; restore registers

    add esp, 8               ; remove error code + interrupt number
    iret
