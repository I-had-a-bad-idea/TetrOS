
mov ah, 0x0e    ; function number = 0Eh : Display Character
mov al, 'H'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'E'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'L'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'L'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'O'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, ' '
int 0x10
mov al, 'W'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'O'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'R'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'L'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service
mov al, 'D'     ; AL = code of character to display
int 0x10        ; call INT 10h, BIOS video service


jmp $
; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before
db 0x55, 0xaa