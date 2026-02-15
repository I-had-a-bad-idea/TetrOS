; enable character "printing"
mov ah, 0x0e    ; function number = 0Eh : Display Character

; set al to 64 (1 before 'A')
mov al, 64
print_alphabet:
    ; increase al by one
    inc al          ; AL = code of character to display
    int 0x10        ; call INT 10h, BIOS video service

    cmp al, 90
    jne print_alphabet

jmp $
; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before
db 0x55, 0xaa