[org 0x7c00] ; set origin to offset
KERNEL_LOCATION equ 0x1000 ; set kernel memory address
      
mov [BOOT_DISK], dl        ; save drive number         
xor ax, ax                 ; ax = 0     
mov es, ax                 ; es = 0
mov ds, ax                 ; ds = 0

mov bp, 0x8000             ; set base pointer
mov sp, bp                 ; set stack pointer 


; load kernel from disk
mov bx, KERNEL_LOCATION   ; offset where kernel will be loaded 
mov dh, 2    ; number of sectors to read

mov ah, 0x02         ; read floppy/hard disk in CHS mode
mov al, dh           ; number of sectors to read
mov ch, 0x00         ; cylinder number = 0
mov dh, 0x00         ; head number = 0
mov cl, 0x02         ; sector number = 2 (sector 1 is bootloader)
mov dl, [BOOT_DISK]  ; drive number
int 0x13             ; read from disk         ; no error management

                                    
mov ah, 0x0          ; set video mode
mov al, 0x3          ; set to text mode
int 0x10             ; call interrupt

CODE_SEG equ GDT_code - GDT_start ; offset of descriptor
DATA_SEG equ GDT_data - GDT_start ; offset of descriptor

cli ; disable all interrupts
lgdt [GDT_descriptor] ; load GDT


; change to 32 bit protected mode
mov eax, cr0 ; move cr0 into eax
or eax, 1 ; change last bit (enable)
mov cr0, eax ; move eax back to cr0
jmp CODE_SEG:start_protected_mode ; jmp to code segment (far jump)

jmp $
                                    
; Global-Descriptor-Table                    
GDT_start:
    GDT_null:  
        dd 0x0  ; four times 00000000
        dd 0x0  ; four times 00000000

    GDT_code:
        dw 0xffff ; segment limit
        dw 0x0    ; base address (first 16 bits)
        db 0x0    ; base adress (next 8 bitss)
        db 0b10011010 ; Present (segment is use), Privilege (highest privilege), Type flags (code, conforming, readable, accessed)
        db 0b11001111 ; Other (granularity, 32 bits, 64 bits, AVL)
        db 0x0    ; base address (last 8 bits)

    GDT_data:
        dw 0xffff ; segment limit
        dw 0x0    ; 16 bits +
        db 0x0    ; 8 bits = 24 bits (base)
        db 0b10010010 ; Present (segment is use), Privilege (highest privilege), Type flags (code, direction, writable, accessed)
        db 0b11001111 ; Other (granularity, 32 bits, 64 bits, AVL)
        db 0x0    ; last 8 bits of base (total 32 bits)

GDT_end:

; GDT Descriptor
GDT_descriptor:
    dw GDT_end - GDT_start - 1 ; size of descriptor
    dd GDT_start               ; start of GDT (base address)


; 32 bit protected mode

[bits 32]
start_protected_mode:
    ; flat memory model
    mov ax, DATA_SEG
    mov ds, ax  ; set data segment
    mov ss, ax  ; set stack segment
    mov es, ax  ; set extra segement
    mov fs, ax  ; set fs
    mov gs, ax  ; set gs

    mov ebp, 0x90000  ; set 32 bit stack pointer
    mov esp, ebp      ; init 32 bit stack

    jmp KERNEL_LOCATION  ; jump to loaded kernel

; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before  ; fill in remaining space with 0
db 0x55, 0xaa  ; boot signature