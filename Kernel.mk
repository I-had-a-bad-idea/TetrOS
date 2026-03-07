# Default target
all: binaries/TetrOS.bin

# Create build directories if needed
kernel_object_files:
	mkdir -p kernel_object_files

binaries:
	mkdir -p binaries

# Compile kernel.c
kernel_object_files/kernel.o: src/kernel/kernel.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Compile idt.c
kernel_object_files/idt.o: src/kernel/idt.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Compile isr.c
kernel_object_files/isr.o: src/kernel/isr.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Compile isr_gen.c
kernel_object_files/isr_gen.o: src/kernel/isr_gen.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Compile io.c
kernel_object_files/io.o: src/kernel/io.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Compile pic.c
kernel_object_files/pic.o: src/kernel/pic.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Compile irq.c
kernel_object_files/irq.o: src/kernel/irq.c | kernel_object_files
	i686-elf-gcc -ffreestanding -m32 -g -c $< -o $@

# Link all object files to full_kernel.bin
binaries/full_kernel.bin: binaries/kernel_entry.o kernel_object_files/kernel.o kernel_object_files/idt.o kernel_object_files/isr.o binaries/isr.o kernel_object_files/isr_gen.o binaries/io.o kernel_object_files/io.o kernel_object_files/pic.o kernel_object_files/irq.o | binaries
	i686-elf-ld -o $@ -T linker.ld $^ --oformat binary

# Combine into TetrOS.bin
binaries/TetrOS.bin: binaries/boot.bin binaries/full_kernel.bin  binaries/zeroes.bin
	cat $^ > $@

# Clean
clean:
	rm -rf kernel_object_files/*.o binaries/*.bin