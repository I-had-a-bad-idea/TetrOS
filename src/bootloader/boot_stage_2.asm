[org 0x8000] ; set origin to offset
[bits 16]

KERNEL_LOCATION equ 0x1000 ; set kernel memory address
FRAMEBUFFER_INFO_LOCATION equ 0x9000

xor ax, ax                 ; ax = 0     
mov es, ax                 ; es = 0
mov ds, ax                 ; ds = 0

; VBE setup
mov ax, 0x4F00 ; get controller info
mov di, vbe_info_block
int 0x10
cmp ax, 0x004F
jne vbe_fail

; get a mode (hardcoded) # TODO: dont hardcode this
mov ax, 0x4F01
mov cx, 0x118 ; 1024*768*24
mov di, vbe_mode_info_block
int 0x10
cmp ax, 0x004F
jne vbe_fail 

; set the mode
mov ax, 0x4F02
mov bx, 0x4118 ; 0x4000 = linear framebuffer
int 0x10
cmp ax, 0x004F
jne vbe_fail

; store the vbe mode info
mov ax, [vbe_mode_info_block + 26] ; pitch
mov [FRAMEBUFFER_INFO_LOCATION + 0], ax
mov ax, [vbe_mode_info_block + 18] ; width
mov [FRAMEBUFFER_INFO_LOCATION + 2], ax
mov ax, [vbe_mode_info_block + 20] ; height
mov [FRAMEBUFFER_INFO_LOCATION + 4], ax
mov eax, [vbe_mode_info_block + 28] ; framebuffer addr
mov [FRAMEBUFFER_INFO_LOCATION + 8], eax
mov al, [vbe_mode_info_block + 25] ; bpp (color depth)
mov [FRAMEBUFFER_INFO_LOCATION + 12], al


; Load kernel from disk
mov bx, KERNEL_LOCATION   ; offset where kernel will be loaded 
mov dh, 40    ; number of sectors to read (currently reads too much to avoid future problems)

mov ah, 0x02         ; read floppy/hard disk in CHS mode
mov al, dh           ; number of sectors to read
mov ch, 0x00         ; cylinder number = 0
mov dh, 0x00         ; head number = 0
mov cl, 0x7           ; sector number = 8 (sector 1 is stage1 and sections 2-6 is stage2 )
; dl already contains drive number
int 0x13             ; read from disk
; error management
jc disk_error        ; jmp to disk_error if cf is 1 (error)


; GDT
cli ; disable all interrupts
lgdt [GDT_descriptor] ; load GDT

; Change to 32 bit protected mode
mov eax, cr0 ; move cr0 into eax
or eax, 1 ; change last bit (enable)
mov cr0, eax ; move eax back to cr0
jmp CODE_SEG:start_protected_mode ; jmp to code segment (far jump)


; Error handling

; prints a string stored in ds:si (only works in real-mode )
print_string:
    mov ah, 0x0e   ; function number = 0Eh : Display Character
    mov al, [ds:si] ; get char 
    cmp al, 0 ; check if end of string
    je print_string_end    ; jump to print_string_end label
    int 0x10 ; BIOS interrupt for "printing"
    inc si   ; next char (increment pointer)
    jmp print_string ; loop through all of string
print_string_end:
    ret

disk_read_error_msg:
    db "Reading disk failed!", 0

vbe_fail_error_msg:
    db "Setting up VBE failed!", 0

vbe_fail:
    mov si, vbe_fail_error_msg   ; set error message
    call print_string            ; call print (prints si (the error msg))
    jmp $                        ; halt execution

disk_error:
    mov si, disk_read_error_msg  ; set error message
    call print_string            ; call print (prints si (the error msg))
    jmp $                        ; halt execution


; Global-Descriptor-Table 
CODE_SEG equ GDT_code - GDT_start ; offset of descriptor
DATA_SEG equ GDT_data - GDT_start ; offset of descriptor           
GDT_start:
    GDT_null:  
        dd 0x0  ; four times 00000000
        dd 0x0  ; four times 00000000

    GDT_code:
        dw 0xffff ; segment limit
        dw 0x0    ; base address (first 16 bits)
        db 0x0    ; base adress (next 8 bitss)
        db 0b10011010 ; 
                      ; 1 = Present (segment is use)
                      ; 00 = Privilege (highest privilege)
                      ; 1 = code
                      ; 1 = executable
                      ; 0 = not conforming
                      ; 1 = readable
                      ; 0 = not accessed
        db 0b11001111 ; Flags:
                      ; 1 = granularity
                      ; 1 = 32 bits
                      ; 0 = not 64 bits
                      ; 0 = not AVL
        db 0x0    ; base address (last 8 bits)

    GDT_data:
        dw 0xffff ; segment limit
        dw 0x0    ; base address (first 16 bits)
        db 0x0    ; base adress (next 8 bitss)
        db 0b10010010 ; Type flags (code, direction, writable, accessed)
                      ; 1 = Present (segment is use)
                      ; 00 = Privilege (highest privilege)
                      ; 1 = data
                      ; 0 = not executable
                      ; 0 = expand up
                      ; 1 = writable
                      ; 0 = not accessed
        db 0b11001111 ; Flags (same as code segment)
        db 0x0    ; base address (last 8 bits)

GDT_end:

; GDT Descriptor
GDT_descriptor:
    dw GDT_end - GDT_start - 1 ; size of descriptor
    dd GDT_start               ; start of GDT (base address)


; VBE stuff
vbe_info_block:
    times 512 db 0

vbe_mode_info_block:
    times 256 db 0

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

    mov ebp, 0x200000  ; set 32 bit stack pointer
    mov esp, ebp      ; init 32 bit stack

    jmp KERNEL_LOCATION  ; jump to loaded kernel



times (5 * 512) - ($ - $$) db 0  ; use exactly 5 sectors (so loading the kernel works)