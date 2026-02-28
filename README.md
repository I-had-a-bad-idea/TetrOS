# TetrOS
A simple OS to run Tetris

## Installation
You will need to install NASM, a cross-compiler and some VM to use this.

Then use the Makefiles:

```bash
make -f Assembly.mk

make -f Kernel.mk
```

For VirtualBox:
```bash
make -f VirtualBox.mk
```

All together:

```bash
make -f Assembly.mk
make -f Kernel.mk
make -f VirtualBox.mk
```


`kernel_entry.asm` will be directly behind `boot.asm`.