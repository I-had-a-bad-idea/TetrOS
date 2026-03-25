all: binaries/boot_stage_1.bin binaries/boot_stage_2.bin binaries/zeroes.bin binaries/kernel_entry.o binaries/isr.o binaries/io.o

binaries/boot_stage_1.bin:
	nasm src/bootloader/boot_stage_1.asm -f bin -o binaries/boot_stage_1.bin

binaries/boot_stage_2.bin:
	nasm src/bootloader/boot_stage_2.asm -f bin -o binaries/boot_stage_2.bin

binaries/zeroes.bin:
	nasm src/bootloader/zeroes.asm -f bin -o binaries/zeroes.bin

binaries/kernel_entry.o:
	nasm src/bootloader/kernel_entry.asm -f elf32 -o binaries/kernel_entry.o

binaries/isr.o:
	nasm src/kernel/isr.asm -f elf32 -o binaries/isr.o

binaries/io.o:
	nasm src/kernel/io.asm -f elf32 -o binaries/io.o