all: disk.img

disk.img:
	dd if=/dev/zero of=disk.img bs=1M count=1
	dd if=TetrOS.bin of=disk.img conv=notrunc