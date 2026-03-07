global outb
outb:
    [bits 32]
    mov dx, [esp + 4]  ; port
    mov al, [esp + 8]  ; value
    out dx, al         ; put byte from al to dx
    ret

global inb
inb:
    [bits 32]
    mov dx, [esp + 4]  ; port
    xor eax, eax       ; empty eax
    in al, dx          ; read from port
    ret

