# Tool information
TOOL_DIR		= C:\CodeSourcery\Sourcery G++ Lite
VERSION			= 4.8.1
TARGET			= arm-none-eabi

# Project name & Link script
OUT_FILE_NAME 	= rom_0x08003000
LDS_FILE_NAME	= rom_0x08003000.lds

# Compiler & Linker Option, Cortex-M3 & No FPU
CFLAGS			= -mcpu=cortex-m3 -c -g -O3 -Wall -mthumb -msoft-float -fno-builtin -funsigned-char
CFLAGS		    += -fno-strict-aliasing -fno-common -pipe -mapcs -mno-thumb-interwork 
LDFLAGS			= --cref -Bstatic -nostdlib -p -EL -lm -L. -llcd

# Output File definition
OUT_BIN_FILE	= $(OUT_FILE_NAME).bin
OUT_ELF_FILE	= $(OUT_FILE_NAME).elf
OUT_MAP_FILE	= $(OUT_FILE_NAME).map
OUT_DUMP_FILE	= $(OUT_FILE_NAME).dmp

# Tool setting
AS				= "$(TOOL_DIR)/bin/$(TARGET)-as"
CC				= "$(TOOL_DIR)/bin/$(TARGET)-gcc"
LD 				= "$(TOOL_DIR)/bin/$(TARGET)-ld"
OBJCOPY			= "$(TOOL_DIR)/bin/$(TARGET)-objcopy"
OBJDUMP			= "$(TOOL_DIR)/bin/$(TARGET)-objdump"

# Source & Header File 
CSRC 			= $(wildcard *.c) 
CPPSRC			= $(wildcard *.cpp)
ASRC 			= $(wildcard *.s)
HEADER 			= $(wildcard *.h)
OBJS    		= $(CSRC:.c=.o) $(ASRC:.s=.o) $(CPPSRC:.cpp=.o)
DUMP 			= $(wildcard *.txt)

# Library and include folder
C_DIR			= $(TOOL_DIR)/$(TARGET)
GCC_DIR			= $(TOOL_DIR)/lib/gcc/$(TARGET)/$(VERSION)
LIB_OPTION		= -L "$(C_DIR)/lib/thumb2" -L "$(GCC_DIR)/thumb2" -lc -lgcc -lstdc++
INCLUDE			= -nostdinc  -I. -I "$(C_DIR)/include" -I "$(GCC_DIR)/include" 

all : $(OUT_BIN_FILE)

$(OUT_BIN_FILE): $(OUT_ELF_FILE)
	$(OBJCOPY) $(OUT_ELF_FILE) $(OUT_BIN_FILE) -O binary

$(OUT_ELF_FILE): $(OBJS) 
	$(LD) $(OBJS) -o $(OUT_ELF_FILE) $(LDFLAGS) -Map $(OUT_MAP_FILE) $(LIB_OPTION) -T $(LDS_FILE_NAME)

$(OBJS): $(ASRC) $(CSRC) $(CPPSRC) $(HEADER)
	$(CC) $(ASRC) $(CSRC) $(CPPSRC) $(INCLUDE) $(CFLAGS) 

clean :
	rm -f $(OUT_BIN_FILE)
	rm -f $(OUT_ELF_FILE)
	rm -f $(OUT_MAP_FILE)
	rm -f $(OBJS)
	rm -f $(DUMP)