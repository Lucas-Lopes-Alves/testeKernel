CAMINHO?=isos/teste.iso
ISO?=teste

build32:
	i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernel.c -o bin/kernel.o
	i686-elf-ld -m elf_i386 -T kernel.ld bin/kernel.o -o iso/boot/kernel.bin
	grub-mkrescue iso -o isos/$(ISO).iso

emulate:
	qemu-system-x86_64 -cdrom $(CAMINHO)