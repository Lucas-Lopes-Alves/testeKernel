PATH?=isos/teste.iso
ISO?=teste
DESTINY?=isos

build: kernel.c boot.s
	i686-elf-gcc -ffreestanding -nostdlib -c kernel.c -o bin/kernel.o
	i686-elf-as boot.s -o bin/boot.o
	i686-elf-ld -T kernel.ld bin/kernel.o bin/boot.o -o iso/boot/kernel.elf
	grub-mkrescue iso -o $(DESTINY)/$(ISO).iso

emulate:
	qemu-system-x86_64 -cdrom $(PATH)

buildwsl:
	i686-elf-gcc.exe -ffreestanding -nostdlib -c kernel.c -o bin/kernel.o
	i686-elf-as.exe boot.s -o bin/boot.o
	i686-elf-ld.exe -T kernel.ld bin/kernel.o bin/boot.o -o iso/boot/kernel.elf
	grub-mkrescue iso -o isos/$(ISO).iso
