[org 0x7c00] ; set origin to offset

; enable character "printing"
mov ah, 0x0e    ; function number = 0Eh : Display Character

mov bx, hello_world_string ; set bx to the flag pointer

print_string:
    mov al, [bx] ; get char 
    cmp al, 0 ; check if end of string
    je end    ; jump to end label
    int 0x10 ; BIOS interrupt for "printing"
    inc bx   ; next char (increment pointer)
    jmp print_string ; loop through all of string

end:


jmp $
hello_world_string:
    db "Hello World", 0

; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before
db 0x55, 0xaa