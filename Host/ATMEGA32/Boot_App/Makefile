#Compiler and target program
CXX := avr-gcc
MCU_TYPE := atmega32
CFLAGS := -c -Wall -std=c99 -mmcu=$(MCU_TYPE) -Os
LFLAGS := -N -nostdlib -T atmega32_boot_ls.ld -Wl,-Map=BOOT_APP.map atmega32_boot_startup.o atmega32_boot_main.o BOOT_UART.o EEPROM.o -o BOOT_APP.elf
OBJS := .\*.o

.PHONY: clean all link build

#Directories forcvl .o and .d files
OBJDIR := ./
DEPDIR := $(OBJDIR)

#All sources, objects, and dependancy files
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(OBJDIR)%.o)
DEPS := $(SRCS:%.c=$(DEPDIR)%.d)

#Root target for linking compiled .o files into one binary
all: $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@
	
#Target to compile all .cpp files, generating .d files in the process
$(OBJDIR)%.o: %.c $(DEPDIR)%.d
	$(CXX) $(CFLAGS) -MMD -MT $@ -MP -MF $(DEPDIR)$*.d -o $@ $<


#Empty targets for handling missing .d files
$(DEPS):

# delete .d files, compiled .o files, and linked binary leaving just the code
clean:
	$(RM) $(wildcard *.o)
	$(RM) $(wildcard *.d)
	$(RM) $(wildcard *.elf)
	$(RM) $(wildcard *.hex)
	$(RM) $(wildcard *.bin)
	$(RM) $(wildcard *.map)

link: all
	$(CXX) $(LFLAGS)
	@echo "Linking Done Successfully............"

# generate .o file from .c file.
%.o: %.c
	$(CXX) $(CFLAGS) -o $@ $<

# include all the dependency files
include $(wildcard $(DEPS))
