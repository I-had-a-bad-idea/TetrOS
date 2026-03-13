# Kernel object files
OBJ = \
kernel_object_files/kernel.o \
kernel_object_files/idt.o \
kernel_object_files/isr.o \
kernel_object_files/isr_gen.o \
kernel_object_files/io.o \
kernel_object_files/pic.o \
kernel_object_files/irq.o

# Tetris source files
TETRIS_SRC := $(wildcard src/tetris/*.c)

# Object files generated from them
TETRIS_OBJ := $(patsubst src/tetris/%.c,kernel_object_files/%.o,$(TETRIS_SRC))

CFLAGS = -ffreestanding -m32 -g -I src

all: TetrOS.bin

# Ensure directories exist
kernel_object_files:
	mkdir -p kernel_object_files

binaries:
	mkdir -p binaries


# Compile kernel source files
kernel_object_files/%.o: src/kernel/%.c | kernel_object_files
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# Compile Tetris source files
kernel_object_files/%.o: src/tetris/%.c | kernel_object_files
	i686-elf-gcc $(CFLAGS) -c $< -o $@


# Link full kernel
binaries/full_kernel.bin: \
	binaries/kernel_entry.o \
	$(OBJ) \
	binaries/isr.o \
	binaries/io.o \
	$(TETRIS_OBJ) \
	| binaries

	i686-elf-ld -o $@ -T linker.ld $^ --oformat binary


# Combine bootloader + kernel
TetrOS.bin: binaries/boot.bin binaries/full_kernel.bin binaries/zeroes.bin | binaries
	cat $^ > $@


# Clean
clean:
	rm -rf kernel_object_files/*.o binaries/*.bin