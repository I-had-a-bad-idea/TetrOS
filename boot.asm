jmp $
; define 510 bytes + 2 bytes after = 512 bytes (boot sector)
times 510 - ($-$$) db 0 ; ($-$$) = the code before
db 0x55, 0xaa