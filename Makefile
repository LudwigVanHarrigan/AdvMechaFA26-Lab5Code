# Makefile

# Toolchain definitions
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Hardware architecture flags specific to the Nucleo-F446ZE
MCU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Compilation flags
# -I. looks for headers in the root folder, -Ihardware looks inside the hardware folder
CFLAGS  = $(MCU_FLAGS) -O0 -g -Wall -ffreestanding -I. -Ihardware

# Linker flags
LDFLAGS = -T linker.ld -nostdlib

# Explicitly list all your C source files, including their subfolder paths
C_SOURCES = main.c \
			stm32f446xx_system_mort.c \
			hardware/stm32f446xx_gpio_mort.c \
			hardware/stm32f446xx_rcc_mort.c \
			hardware/stm32f446xx_timer_mort.c \
			hardware/stm32f446xx_usart3_mort.c \
			hardware/stm32f446xx_interruptcontroller_mort.c \
			hardware/stm32f446xx_dma_controller_mort.c \
			hardware/stm32f446xx_adc_mort.c \
			stupid_delay.c \
			A2.c \
			P2.c \
			encoder.c

# Your assembly startup file
S_SOURCES = startup_stm32f4xx.s

# Convert source file names into object file names (.o)
# The "patsubst" magic ensures objects mirror the source paths cleanly
OBJECTS = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)

# Default target rule
all: main.bin

# Link all object files together into an ELF file
main.elf: $(OBJECTS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o main.elf

# Strip debugging structures to generate the final flashable raw binary
main.bin: main.elf
	$(OBJCOPY) -O binary main.elf main.bin

# Compile C source files into object files (handles root and subfolders seamlessly)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble your startup file
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to clear build artifacts across all folders
clean:
	rm -f *.o *.elf *.bin hardware/*.o 
