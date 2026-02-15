[org 0x7c00] ; set origin to offset
      
mov [BOOT_DISK], dl                 

CODE_SEG equ GDT_code - GDT_start ; offset of descriptor
DATA_SEG equ GDT_data - GDT_start ; offset of descriptor

cli ; disable all interrupts
lgdt [GDT_descriptor] ; load GDT

; change to 32 bit protected mode
mov eax, cr0 ; move cr0 into eax
or eax, 1 ; change last bit
mov cr0, eax ; move eax back to cr0
jmp CODE_SEG:start_protected_mode ; jmp to code segment (far jump)

jmp $
                                    
; Global-Descriptor-Table                    
GDT_start:      ; must be at the end of real mode code
    GDT_null:  
        dd 0x0  ; four times 00000000
        dd 0x0  ; four times 00000000

    GDT_code:
        dw 0xffff ; Limit (max. in 20 bits)
        dw 0x0    ; 16 bits +
        db 0x0    ; 8 bits = 24 bits (base)
        db 0b10011010 ; Present (segment is use), Privilege (highest privilege), Type flags (code, conforming, readable, accessed)
        db 0b11001111 ; Other (granularity, 32 bits, 64 bits, AVL)
        db 0x0    ; last 8 bits of base (total 32 bits)

    GDT_data:
        dw 0xffff ; Limit (max. in 20 bits)
        dw 0x0    ; 16 bits +
        db 0x0    ; 8 bits = 24 bits (base)
        db 0b10010010 ; Present (segment is use), Privilege (highest privilege), Type flags (code, direction, writable, accessed)
        db 0b11001111 ; Other (granularity, 32 bits, 64 bits, AVL)
        db 0x0    ; last 8 bits of base (total 32 bits)

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1 ; size of descriptor
    dd GDT_start               ; start of GDT


[bits 32]
start_protected_mode:
    ; flat memory model
    mov ax, DATA_SEG
    mov ds, ax ; data segment
    mov ss, ax ; stack segment
    mov es, ax ; extra segement

    mov al, '#' ; char
    mov ah, 0x0f ; white on black
    mov [0xb8000], ax  ; move ax to beginning of video memory
    jmp $

BOOT_DISK: db 0                                     
 
; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before
db 0x55, 0xaa