all: disk.img

disk.img:
	dd if=/dev/zero of=disk.img bs=1M count=1
	dd if=binaries/TetrOS.bin of=disk.img conv=notrunc