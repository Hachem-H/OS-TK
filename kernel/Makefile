OS_NAME = OS-TK

GNU-EFI_DIR = ../vendor/gnu-efi
OVMF_DIR    = ../vendor/OVMF
 
CC = gcc

CC_FLAGS = -ffreestanding \
		   -fshort-wchar
LD_FLAGS = -T -shared \
		   -Bsymbolic \
		   -nostdlib

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := bin-int

BOOT_EFI := $(GNU-EFI_DIR)/x86_64/bootloader/main.efi
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = $(call rwildcard,$(SRC_DIR),*.c)          
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DIRS = $(wildcard $(SRC_DIR)/*)

env:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
clean:
	rm -p $(BIN_DIR)
	rm -p $(OBJ_DIR)
run:
	qemu-system-x86_64 -drive file=$(BIN_DIR)/$(OS_NAME).img -m 256M -cpu qemu64                               \
					   -drive if=pflash,format=raw,unit=0,file="$(OVMF_DIR)/OVMF_CODE-pure-efi.fd",readonly=on \
					   -drive if=pflash,format=raw,unit=1,file="$(OVMF_DIR)/OVMF_VARS-pure-efi.fd" -net none
build-img: env
	dd if=/dev/zero of=$(BIN_DIR)/$(OS_NAME).img bs=512 count=93750
	mformat -i $(BIN_DIR)/$(OS_NAME).img -f 1440 ::
	mmd -i $(BIN_DIR)/$(OS_NAME).img ::/EFI
	mmd -i $(BIN_DIR)/$(OS_NAME).img ::/EFI/BOOT
	mcopy -i $(BIN_DIR)/$(OS_NAME).img $(BOOT_EFI) ::/EFI/BOOT
	mcopy -i $(BIN_DIR)/$(OS_NAME).img startup.nsh ::