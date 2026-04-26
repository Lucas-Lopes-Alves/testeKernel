PLACE?=isos/kernel.iso
CC=i686-elf-gcc
AS = i686-elf-as
CFLAGS= -ffreestanding -nostdlib -Iinclude
LDFLAGS= -ffreestanding -nostdlib -lgcc

# Saves all the files ending with .c to
# change them from .c to .o
SRCS_C := $(wildcard src/*.c)
SRCS_S := $(wildcard src/*.s)
OBJS := $(patsubst src/%.c, bin/%.o, $(SRCS_C))
OBJS += $(patsubst src/%.s, bin/%.o, $(SRCS_S))
DOBJS := $(patsubst src/%.c, debugBin/D%.o, $(SRCS_C))
DOBJS += $(patsubst src/%.s, debugBin/D%.o, $(SRCS_S))

all: build

#Build section
build: $(OBJS)
	@echo "Linking the object files"
	@$(CC) $(LDFLAGS) -T kernel.ld $(OBJS) -o iso/boot/kernel.elf
	@echo "Done"
	@echo "Creating the iso at $(PLACE)"
	@grub-mkrescue iso -o $(PLACE) > /dev/null 2>&1
	@echo "Done"

# Tells make how to create the C and Assembly 
# object files and where to place them
bin/%.o: src/%.c
	@echo "Compiling the C files"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Done"

bin/%.o: src/%.s
	@echo "Compiling the Assembly files"
	@$(AS) $< -o $@
	@echo "Done"
#End build section

#Debug section
debug: $(DOBJS)
	$(CC) $(LDFLAGS) -T kernel.ld $(DOBJS) -o debug/iso/boot/Dkernel.elf
	grub-mkrescue debug/iso -o isos/Debug.iso

debugBin/D%.o: src/%.c
	$(CC) $(CFLAGS) -g -c $< -o $@

debugBin/D%.o: src/%.s
	$(AS) $< -o $@
#End debug

emulate:
	qemu-system-i386 -cdrom $(PLACE)

clean:
	rm -r /bin

teste:
	@echo $(DOBJS)

.PHONY: debug clean