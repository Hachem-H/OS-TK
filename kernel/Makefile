OS_NAME = OS-TK

GNU-EFI_DIR = ../vendor/gnu-efi
OVMF_DIR    = ../vendor/OVMF
MTOOLS_DIR  = ../vendor/mtools
 
ASM = nasm
CC = gcc
LD = ld

LIBC_DIR     = ../lib/libc
LIBC_INCLUDE = $(LIBC_DIR)/include
GLOB_INCLUDE = ../include

CC_FLAGS = -I$(LIBC_INCLUDE) \
     	   -I$(GLOB_INCLUDE) \
           -ffreestanding    \
		   -fshort-wchar     \
		   -Wall -Wextra     \
		   -I$(SRC_DIR)

LD_FLAGS = -T linker.ld \
		   -Bsymbolic   \
		   -nostdlib    \
		   -static

LIBC_SRC_DIR := $(LIBC_DIR)/src
SRC_DIR      := src
BIN_DIR      := bin
OBJ_DIR      := bin-int

EFS_DIR  := fs-efi
BOOT_EFI := $(GNU-EFI_DIR)/x86_64/bootloader/main.efi

SRCS = $(shell find src -name "*.c")          
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DIRS = $(dir $(OBJS))

ASM_SRCS = $(shell find src -name "*.asm")          
ASM_OBJS = $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.asm.o,$(ASM_SRCS))
ASM_DIRS = $(dir $(ASM_OBJS))

LIBC_SRCS = $(shell find $(LIBC_SRC_DIR) -name "*.c")          
LIBC_OBJS = $(patsubst $(LIBC_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIBC_SRCS))
LIBC_DIRS = $(dir $(LIBC_OBJS))

CUSTOM_OBJS = $(shell find ../bin -name "*.o")
OBJECTS = $(OBJS) $(ASM_OBJS) $(LIBC_OBJS)

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%.c,$@) -o $@
$(ASM_OBJS): $(OBJ_DIR)/%.asm.o : $(SRC_DIR)/%.asm
	$(ASM) -f elf64 $(patsubst $(OBJ_DIR)/%.asm.o,$(SRC_DIR)/%.asm,$@) -o $@

$(LIBC_OBJS): $(OBJ_DIR)/%.o : $(LIBC_SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $(patsubst $(OBJ_DIR)/%.o,$(LIBC_SRC_DIR)/%.c,$@) -o $@
$(OBJ_DIR)/Core/Interrupts.o: $(SRC_DIR)/Core/Interrupts.c
	$(CC) $(CC_FLAGS) -mno-red-zone -mgeneral-regs-only -c $(SRC_DIR)/Core/Interrupts.c -o $(OBJ_DIR)/Core/Interrupts.o

env:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)/iso
	@mkdir -p $(DIRS)
clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(OBJ_DIR)
run:
	qemu-system-x86_64 -drive file=$(BIN_DIR)/$(OS_NAME).img,format=raw -m 256M -machine q35 -cpu qemu64 -net none \
	                   -drive if=pflash,format=raw,unit=0,file="$(OVMF_DIR)/OVMF_CODE-pure-efi.fd",readonly=on     \
	                   -drive if=pflash,format=raw,unit=1,file="$(OVMF_DIR)/OVMF_VARS-pure-efi.fd",readonly=on

kernel: env $(OBJECTS) link
link:
	$(LD) $(LD_FLAGS) -o $(BIN_DIR)/kernel.elf $(CUSTOM_OBJS) $(OBJECTS) 

build-img: kernel
	dd if=/dev/zero of=$(BIN_DIR)/$(OS_NAME).img bs=512 count=93750
	$(MTOOLS_DIR)/mformat -i $(BIN_DIR)/$(OS_NAME).img -f 1440 ::
	$(MTOOLS_DIR)/mmd -i $(BIN_DIR)/$(OS_NAME).img ::/EFI
	$(MTOOLS_DIR)/mmd -i $(BIN_DIR)/$(OS_NAME).img ::/EFI/BOOT
	$(MTOOLS_DIR)/mcopy -i $(BIN_DIR)/$(OS_NAME).img $(BOOT_EFI) ::/EFI/BOOT
	$(MTOOLS_DIR)/mcopy -i $(BIN_DIR)/$(OS_NAME).img $(EFS_DIR)/startup.nsh ::
	$(MTOOLS_DIR)/mcopy -i $(BIN_DIR)/$(OS_NAME).img $(EFS_DIR)/zap-light16.psf ::
	$(MTOOLS_DIR)/mcopy -i $(BIN_DIR)/$(OS_NAME).img $(BIN_DIR)/kernel.elf ::
build-iso: kernel
	rm -rf $(BIN_DIR)/iso/*.iso
	rm -rf $(BIN_DIR)/iso/*.imgcp $(BIN_DIR)/$(OS_NAME).img $(BIN_DIR)/iso
	xorriso -as mkisofs -R -f -e $(OS_NAME).img -no-emul-boot -o $(BIN_DIR)/iso/$(OS_NAME).iso /$(BIN_DIR)
