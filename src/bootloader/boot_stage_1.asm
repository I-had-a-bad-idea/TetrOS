[org 0x7c00] ; set origin to offset
STAGE_2_LOAD_ADDRESS equ 0x8000
      
mov [BOOT_DISK], dl        ; save drive number         
xor ax, ax                 ; ax = 0     
mov es, ax                 ; es = 0
mov ds, ax                 ; ds = 0

mov bp, 0x8000             ; set base pointer
mov sp, bp                 ; set stack pointer 


; load stage2 from disk
mov bx, STAGE_2_LOAD_ADDRESS   ; offset where stage2 will be loaded 
mov dh, 5    ; number of sectors to read

mov ah, 0x02         ; read floppy/hard disk in CHS mode
mov al, dh           ; number of sectors to read
mov ch, 0         ; cylinder number = 0
mov dh, 0         ; head number = 0
mov cl, 2         ; sector number = 2 (sector 1 is bootloader)
mov dl, [BOOT_DISK]  ; drive number
int 0x13             ; read from disk
; error management
jc disk_error        ; jmp to disk_error if cf is 1 (error)
jmp disk_sucess      ; jmp to disk_success if no error
disk_error:
    jmp $                        ; halt execution

disk_sucess:        ; if sucess continue
    mov dl, [BOOT_DISK]
    jmp 0x0000:STAGE_2_LOAD_ADDRESS
        
BOOT_DISK:
    db 0

; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before  ; fill in remaining space with 0
db 0x55, 0xaa  ; boot signature