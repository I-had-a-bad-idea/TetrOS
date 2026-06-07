# Directories
BIN_DIR := binaries
OBJ_DIR := kernel_object_files

# Compiler settings
CFLAGS := -ffreestanding -m32 -I src -Os

# Kernel object files
OBJ := \
	$(OBJ_DIR)/kernel.o \
	$(OBJ_DIR)/idt.o \
	$(OBJ_DIR)/isr.o \
	$(OBJ_DIR)/isr_gen.o \
	$(OBJ_DIR)/io.o \
	$(OBJ_DIR)/pic.o \
	$(OBJ_DIR)/irq.o

# Tetris sources
TETRIS_SRC := $(wildcard src/tetris/*.c)
TETRIS_OBJ := $(patsubst src/tetris/%.c,$(OBJ_DIR)/%.o,$(TETRIS_SRC))

# Assembly outputs
BOOT_BIN := \
	$(BIN_DIR)/boot_stage_1.bin \
	$(BIN_DIR)/boot_stage_2.bin \
	$(BIN_DIR)/zeroes.bin

ASM_OBJ := \
	$(BIN_DIR)/kernel_entry.o \
	$(BIN_DIR)/isr.o \
	$(BIN_DIR)/io.o

# Default target: compile everything
all: TetrOS.bin

# Directories
$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Bootloader binaries
$(BIN_DIR)/boot_stage_1.bin: src/bootloader/boot_stage_1.asm | $(BIN_DIR)
	nasm $< -f bin -o $@

$(BIN_DIR)/boot_stage_2.bin: src/bootloader/boot_stage_2.asm | $(BIN_DIR)
	nasm $< -f bin -o $@

$(BIN_DIR)/zeroes.bin: src/bootloader/zeroes.asm | $(BIN_DIR)
	nasm $< -f bin -o $@

# Assembly object files
$(BIN_DIR)/kernel_entry.o: src/bootloader/kernel_entry.asm | $(BIN_DIR)
	nasm $< -f elf32 -o $@

$(BIN_DIR)/isr.o: src/kernel/isr.asm | $(BIN_DIR)
	nasm $< -f elf32 -o $@

$(BIN_DIR)/io.o: src/kernel/io.asm | $(BIN_DIR)
	nasm $< -f elf32 -o $@

# Kernel C sources
$(OBJ_DIR)/%.o: src/kernel/%.c | $(OBJ_DIR)
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# Tetris C sources
$(OBJ_DIR)/%.o: src/tetris/%.c | $(OBJ_DIR)
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# Link kernel
$(BIN_DIR)/full_kernel.bin: \
	$(BIN_DIR)/kernel_entry.o \
	$(OBJ) \
	$(BIN_DIR)/isr.o \
	$(BIN_DIR)/io.o \
	$(TETRIS_OBJ) \
	| $(BIN_DIR)
	i686-elf-ld -o $@ -T linker.ld $^ --oformat binary

# Build bootable binary
TetrOS.bin: \
	$(BIN_DIR)/boot_stage_1.bin \
	$(BIN_DIR)/boot_stage_2.bin \
	$(BIN_DIR)/full_kernel.bin \
	$(BIN_DIR)/zeroes.bin
	cat $^ > $@

# Explicit floppy image target
image: TetrOS.img

TetrOS.img: TetrOS.bin
	dd if=/dev/zero of=$@ bs=1M count=1
	dd if=$< of=$@ conv=notrunc

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) TetrOS.bin TetrOS.img