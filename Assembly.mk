all: binaries/boot.bin binaries/zeroes.bin binaries/kernel_entry.o

binaries/boot.bin:
	nasm src/bootloader/boot.asm -f bin -o binaries/boot.bin

binaries/zeroes.bin:
	nasm src/bootloader/zeroes.asm -f bin -o binaries/zeroes.bin

binaries/kernel_entry.o:
	nasm src/bootloader/kernel_entry.asm -f elf -o binaries/kernel_entry.o