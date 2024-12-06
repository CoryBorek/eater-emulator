
#
# Directory that the final file is put into
#
TARGET_DIR ?= .

#
# mode can be either main or gui.
#
MODE?=main

# C Emulator
CC? = gcc

# Basic C arguments
CARGS= -Wall -Iinclude -g

# C Objects
OBJS =	obj/modes.o \
	obj/via65c22.o \
	obj/HD44870U.o \
	obj/acia65c51.o \
	obj/bus.o \
	obj/ram.o \
	obj/eeprom26c256.o \
	obj/cpu/addressing.o \
	obj/cpu/common.o \
	obj/cpu/instr_0.o \
	obj/cpu/instr_1.o \
	obj/cpu/instr_2.o \
	obj/cpu/instr_3.o \
	obj/cpu/instr_4.o \
	obj/cpu/instr_5.o \
	obj/cpu/instr_6.o \
	obj/cpu/instr_7.o \
	obj/cpu/instr_8.o \
	obj/cpu/instr_9.o \
	obj/cpu/instr_A.o \
	obj/cpu/instr_B.o \
	obj/cpu/instr_C.o \
	obj/cpu/instr_D.o \
	obj/cpu/instr_E.o \
	obj/cpu/instr_F.o \
	obj/cpu6502.o


#
# GUI mode C arguments
#
ifeq ($(MODE),gui)
CARGS+=-lncurses -DNCURSES
endif

ifeq ($(MODE),main)
CARGS+= -DDEBUG
endif

# Basic outputs
.PHONY: all eater clean

#
# Compile the emulator
#
all: $(TARGET_DIR)/emu eater

$(TARGET_DIR)/emu: src/$(MODE).c $(OBJS)
	@$(CC) $(CARGS) -o $@ $(OBJS) src/$(MODE).c
	@echo Created $@ using mode $(MODE)

#
# Utility Libraries
#
obj/%.o: src/%.c
	@mkdir -p obj/cpu
	@$(CC) $(CARGS) -c -o $@ $<
	@echo CC    $<
#
# Assembly Sources
#
eater:
	@$(MAKE) -C eater
#
# Cleanup
#
clean:
	@rm -rf obj
	@rm -rf $(TARGET_DIR)/emu
	@make -C eater clean
	@echo Cleaned!
