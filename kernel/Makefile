OS_NAME = OS-TK

GNU-EFI_DIR = ../vendor/gnu-efi
OVMF_DIR    = ../vendor/OVMF
 
CC = gcc
LD = ld

CC_FLAGS = -ffreestanding \
		   -fshort-wchar  \
		   -I../include
LD_FLAGS = -T linker.ld \
		   -Bsymbolic   \
		   -nostdlib    \
		   -static

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := bin-int

BOOT_EFI := $(GNU-EFI_DIR)/x86_64/bootloader/main.efi
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = $(call rwildcard,$(SRC_DIR),*.c)          
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DIRS = $(wildcard $(SRC_DIR)/*)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

env:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(OBJ_DIR)
run:
	qemu-system-x86_64 -drive file=$(BIN_DIR)/$(OS_NAME).img,format=raw -m 256M -cpu qemu64 -net none          \
	                   -drive if=pflash,format=raw,unit=0,file="$(OVMF_DIR)/OVMF_CODE-pure-efi.fd",readonly=on \
	                   -drive if=pflash,format=raw,unit=1,file="$(OVMF_DIR)/OVMF_VARS-pure-efi.fd",readonly=on

kernel: env $(OBJS) link
link:
	$(LD) $(LD_FLAGS) -o $(BIN_DIR)/kernel.elf $(OBJS)

build-img: env kernel
	dd if=/dev/zero of=$(BIN_DIR)/$(OS_NAME).img bs=512 count=93750
	mformat -i $(BIN_DIR)/$(OS_NAME).img -f 1440 ::
	mmd -i $(BIN_DIR)/$(OS_NAME).img ::/EFI
	mmd -i $(BIN_DIR)/$(OS_NAME).img ::/EFI/BOOT
	mcopy -i $(BIN_DIR)/$(OS_NAME).img $(BOOT_EFI) ::/EFI/BOOT
	mcopy -i $(BIN_DIR)/$(OS_NAME).img res/startup.nsh ::
	mcopy -i $(BIN_DIR)/$(OS_NAME).img res/zap-light16.psf ::
	mcopy -i $(BIN_DIR)/$(OS_NAME).img $(BIN_DIR)/kernel.elf ::