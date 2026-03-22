all: TetrOS.img

TetrOS.img:
	dd if=/dev/zero of=TetrOS.img bs=1M count=1
	dd if=TetrOS.bin of=TetrOS.img conv=notrunc