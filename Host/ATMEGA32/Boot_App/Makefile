#Compiler and target program
CXX := avr-gcc
CDUMP := avr-objdump
COBJCOPY := avr-objcopy
MCU_TYPE := atmega32
CFLAGS := -c -w -std=c99 -mmcu=$(MCU_TYPE) -Os
LFILE := atmega32_boot_ls.ld
LFLAGS := -N -nostdlib -T $(LFILE) 

.PHONY: clean all boot


#Directories forcvl .o and .d files
OBJDIR := ./
DEPDIR := $(OBJDIR)

#All sources, objects, and dependancy files
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(OBJDIR)%.o)
DEPS := $(SRCS:%.c=$(DEPDIR)%.d)

BOOT: all
	@$(CXX) $(LFLAGS) -Wl,-Map=$@.map $(OBJS) -o $@.elf
	@$(CDUMP) -t -D $@.elf > $@.dump
	@$(COBJCOPY) -O ihex $@.elf $@.hex
	@$(COBJCOPY) -O binary $@.elf $@.bin
	@echo "Building Done Successfully............"

#Root target for linking compiled .o files into one binary
all: $(OBJS)
	@$(CXX) $(CFLAGS) $^ -o $@
	@echo "Compiling Done Successfully............"
	
#Target to compile all .cpp files, generating .d files in the process
$(OBJDIR)%.o: %.c $(DEPDIR)%.d
	@$(CXX) $(CFLAGS) -MMD -MT $@ -MP -MF $(DEPDIR)$*.d -o $@ $<
	@echo "$@ Generated Successfully............"

#Empty targets for handling missing .d files
$(DEPS):

# delete .d files, compiled .o files, and linked binary leaving just the code
clean:
	@$(RM) $(wildcard *.o)
	@$(RM) $(wildcard *.d)
	@$(RM) $(wildcard *.elf)
	@$(RM) $(wildcard *.hex)
	@$(RM) $(wildcard *.bin)
	@$(RM) $(wildcard *.map)
	@$(RM) $(wildcard *.dump)
	@echo "Clean Done Successfully.............."


# generate .o file from .c file.
%.o: %.c
	@$(CXX) $(CFLAGS) -o $@ $<
	@echo "$@ Generated Successfully............"

# include all the dependency files
include $(wildcard $(DEPS))
