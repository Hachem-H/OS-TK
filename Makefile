CC = gcc

CC_FLAGS = -Iinclude/libc/include \
		   -ffreestanding

SRC_DIR := src
OBJ_DIR := bin

SRCS = $(shell find src -name "*.c")          
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DIRS = $(dir $(OBJS))

bootloader:
	cd vendor/gnu-efi; make; make bootloader
bootloader-clean:
	cd vendor/gnu-efi; make clean; cd ../../

kernel-env:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DIRS)
kernel-clean:
	@rm -rf $(DIRS)
	@rm -rf $(OBJ_DIR)
kernel: kernel-env $(OBJS)

kernel-backend: bootloader kernel 
	cd kernel; make build-img
kernel-backend-clean:
	cd kernel; make clean

build: bootloader     \
	   kernel         \
	   kernel-backend
emulate:
	cd kernel; sudo make run
clean: kernel-backend-clean \
	   bootloader-clean     \
       kernel-clean         \

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%.c,$@) -o $@
