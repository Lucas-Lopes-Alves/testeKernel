PLACE?=isos/kernel.iso
CC=i686-elf-gcc
AS = i686-elf-as
CFLAGS= -ffreestanding -nostdlib -Iinclude
LDFLAGS= -ffreestanding -nostdlib -lgcc
# Saves all the files ending with .c to
# change them from .c to .o
SRCS_C := $(wildcard src/*.c)
SRCS_S := $(wildcard src/*.s)
OBJS := $(patsubst src/%.c, obj/%.o, $(SRCS_C))
OBJS += $(patsubst src/%.s, obj/%.o, $(SRCS_S))
DOBJS := $(patsubst src/%.c, debugBin/D%.o, $(SRCS_C))
DOBJS += $(patsubst src/%.s, debugBin/D%.o, $(SRCS_S))

# Checks if the folders exists to create them if not exists and don't create if it exists
$(shell mkdir -p bin debug/iso/boot/grub debugBin include iso/boot/grub isos obj src)

.PHONY: debug clean all build emulate iso

all: build iso

iso: bin/kernel.elf
	@cp bin/kernel.elf iso/boot/kernel.elf
	@echo "Creating the iso at $(PLACE)"
	@grub-mkrescue iso -o $(PLACE) > /dev/null 2>&1
	@echo "Done"

#Build section
build: $(OBJS)
	@echo "Linking the object files"
	@$(CC) $(LDFLAGS) -T kernel.ld $(OBJS) -o bin/kernel.elf
	@echo "Done"

# Tells make how to create the C and Assembly 
# object files and where to place them
obj/%.o: src/%.c
	@echo "Compiling the C files"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Done"

obj/%.o: src/%.s
	@echo "Compiling the Assembly files"
	@$(AS) $< -o $@
	@echo "Done"
#End build section

#Debug section
debug: $(DOBJS)
	@echo "Linking the debug object files"
	@$(CC) $(LDFLAGS) -T kernel.ld $(DOBJS) -o debug/iso/boot/Dkernel.elf
	@echo "Creating the iso at isos/"
	@grub-mkrescue debug/iso -o isos/debug.iso > /dev/null 2>&1
	@echo "Iso created"

debugBin/D%.o: src/%.c
	@echo "Compiling the C files"
	@$(CC) $(CFLAGS) -g -O0 -c $< -o $@
	@echo "Done"

debugBin/D%.o: src/%.s
	@echo "Compiling the Assembly files"
	@$(AS) $< -o $@
	@echo "Done"
#End debug

emulate:
	@qemu-system-i386 -cdrom $(PLACE)

clean:
	@rm -f ./bin/* ./isos/* ./debugBin/* ./iso/boot/*.elf ./debug/iso/boot/*.elf ./obj/*
