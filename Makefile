CC = gcc

LIBC_INCLUDE = include/libc/include

CC_FLAGS = -I$(LIBC_INCLUDE) \
		   -ffreestanding

SRC_DIR := src
OBJ_DIR := bin

SRCS = $(shell find $(SRC_DIR) -name "*.c")          
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DIRS = $(dir $(OBJS))

bootloader:
	cd vendor/gnu-efi; $(MAKE); $(MAKE) bootloader
bootloader-clean:
	cd vendor/gnu-efi; $(MAKE) clean; cd ../../

kernel-env:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DIRS)
kernel-clean:
	@rm -rf $(DIRS)
	@rm -rf $(OBJ_DIR)
kernel: kernel-env $(OBJS)

kernel-backend: bootloader kernel 
	cd kernel; $(MAKE) build-img
kernel-backend-clean:
	cd kernel; $(MAKE) clean

build: kernel-backend
emulate:
	cd kernel; $(MAKE) run
clean: kernel-backend-clean \
	   bootloader-clean     \
       kernel-clean         \

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%.c,$@) -o $@
