######################################
# target
######################################
TARGET = qubitas

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
APP = 1
RELEASE = 0

# Driver code
DRIVER_SOURCES = \
driver/gpio.c \
driver/uart.c \
driver/crc.c \
driver/exti.c \
driver/dma.c \
driver/nvic.c \
driver/timer.c

# library code
LIB_SOURCES = \
lib/io.c \
lib/string.c \
lib/printk.c \

# Memory code
MM_SOURCES = \
mm/heap.c \
mm/mm.c \

# Kernel code
KERNEL_SOURCES = \
kernel/task.c

# Command line code
CMD_SOURCES = \
cmd/cmd.c \
cmd/cmd_shell.c \

# arch dependent code
ARCH_SOURCES = \
arch/arm/init/hw_init.c \
arch/arm/exception.c

# app source code
APP_SOURCES = \
app/svc/svc.c \
app/cs/cs.c

C_SOURCES += $(APP_SOURCES)
C_SOURCES += $(DRIVER_SOURCES)
C_SOURCES += $(LIB_SOURCES)
C_SOURCES += $(MM_SOURCES)
C_SOURCES += $(CMD_SOURCES)

ifeq ($(APP), 1)
$(info "Compiling Context switch")
C_SOURCES += ./test_code/context_switch/main.c
else
$(info "Compiling kernel")
C_SOURCES += main.c
endif

# ASM sources
ASM_SOURCES =  \
startup_stm32f746xx.s

# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-Iinclude \
-Iinclude/qubitas \
-Iinclude/kernel \
-Iinclude/ds \
-Iinclude/mm \
-Iinclude/arch \
-Iinclude/app

#######################################
# Toolchain
#######################################
include mk/toolchain.mk
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# mcu
MCU = $(CPU) -mthumb

# macros for gcc
# AS defines
AS_DEFS = 


# C defines
C_DEFS =  \
-D__QUBITAS__

# optimization
OPT = -Og

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT)
CFLAGS += -fdata-sections -ffunction-sections -nostdlib
CFLAGS += -Werror -Wall

ifeq ($(RELEASE), 1)
ASFLAGS += -Wall
CFLAGS += -Wall
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = qubitas.ld

# libraries
LIBS = -lc -lm
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#######################################
# Download firmware
#######################################
flash:
	st-flash --reset write $(BUILD_DIR)/$(TARGET).bin 0x8000000

erase:
	st-flash erase

check: clean all flash
# *** EOF ***
