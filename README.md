# TetrOS
A simple OS to run Tetris

You will need to install NASM and some VM to use this.

Compile the bootloader using:

```bash
nasm boot.asm -f bin -o binaries/boot.bin
```

Compile the zeroes:

```bash
nasm zeroes.asm -f bin -o binaries/zeroes.bin
```

Compile kernel entry to object file:

```bash
nasm kernel_entry.asm -f elf -o binaries/kernel_entry.o
```

Compile kernel to object file:

```bash
i686-elf-gcc -ffreestanding -m32 -g -c "kernel.c" -o binaries/"kernel.o"
```

Connect kernel entry and kernel:

```bash
i686-elf-ld -o binaries/"full_kernel.bin" -Ttext 0x1000 binaries/"kernel_entry.o" binaries/"kernel.o" --oformat binary
```

Combine them all:

```bash
cat binaries/"boot.bin" binaries/"full_kernel.bin" binaries/"zeroes.bin"  > binaries/"TetrOS.bin"
```

All commands together:

```bash
nasm boot.asm -f bin -o binaries/boot.bin
nasm zeroes.asm -f bin -o binaries/zeroes.bin
nasm kernel_entry.asm -f elf -o binaries/kernel_entry.o

i686-elf-gcc -ffreestanding -m32 -g -c "kernel.c" -o binaries/"kernel.o"
i686-elf-ld -o binaries/"full_kernel.bin" -Ttext 0x1000 binaries/"kernel_entry.o" binaries/"kernel.o" --oformat binary

cat binaries/"boot.bin" binaries/"full_kernel.bin" binaries/"zeroes.bin"  > binaries/"TetrOS.bin"
```

For VirtualBox:
1. Create 1MB raw disk:
```bash
dd if=/dev/zero of=disk.img bs=1M count=1
```
1. Write bootloader to first 512 bytes
```bash
dd if=binaries/TetrOS.bin of=disk.img conv=notrunc
```

Together:

```bash
dd if=/dev/zero of=disk.img bs=1M count=1
dd if=binaries/TetrOS.bin of=disk.img conv=notrunc
```


`kernel_entry.asm` will be directly behind `boot.asm`.