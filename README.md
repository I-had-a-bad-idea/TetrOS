# TetrOS
A simple OS to run Tetris


You will need to install NASM and some VM to use this.

Compile the asembly using:

```bash
nasm -f bin boot.asm -o boot.bin
```

For VirtualBox:
1. Create 1MB raw disk:
```bash
fsutil file createnew disk.img 1048576
```
2. (On Linux) Write bootloader to first 512 bytes
```bash
dd if=boot.bin of=disk.img conv=notrunc
```

3. Convert to VDI
```bash
VBoxManage convertfromraw disk.img boot.vdi --format VDI
```