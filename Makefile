BUILD_DIR=build
OBJ_DIR=obj

ASSEMBLER=nasm
OUTPUT_FILE_FORMAT=elf32
ASM_SRC=start.asm
ASM_OBJ=$(OBJ_DIR)/start.o

CC=gcc
CFLAGS=-m32 -Wall -O -fno-pie -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin 
C_LIBS=./include
# C_SRC=kernel.c
C_SRC=$(wildcard *.c)
# C_OBJ=$(OBJ_DIR)/kc.o
C_OBJ=$(patsubst %.c, $(OBJ_DIR)/%.o, $(C_SRC))

LINKER=ld
LINKER_SCRIPT=link.ld
OUTPUT=kernel-1.0.0
TARGET=$(BUILD_DIR)/$(OUTPUT)

VM=qemu-system-i386

all: build

# Build kernel object files go to /obj and kernel goes to /build
build: $(TARGET) 

# Run with qemu-system-i386 virtual machine
qemu:  
	$(VM) -kernel $(TARGET)

# Link object files with linker script
$(TARGET): $(ASM_OBJ) $(C_OBJ)
	mkdir -p $(BUILD_DIR)
	$(LINKER) -m elf_i386 -T $(LINKER_SCRIPT) -o $(TARGET) $(ASM_OBJ) $(C_OBJ)

# Generate assembler object files with nasm
$(ASM_OBJ): $(ASM_SRC) | $(OBJ_DIR)
	$(ASSEMBLER) -f $(OUTPUT_FILE_FORMAT) $(ASM_SRC) -o $(ASM_OBJ)

# Generate C object files
# $(C_OBJ): $(C_SRC) | $(OBJ_DIR)
# 	mkdir -p $(OBJ_DIR)
# 	$(CC) $(CFLAGS) -I$(C_LIBS) -c $(C_SRC) -o $(C_OBJ)
#
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(C_LIBS) -c $< -o $@

# Create directory to store object files
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Create directory to store kernel build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)


