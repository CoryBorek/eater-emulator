
#
# Directory that the final file is put into
#
TARGET_DIR ?= .

#
# mode can be either main or gui.
#
MODE?=gui

# C Emulator
CC? = gcc

# Basic C arguments
CARGS= -Wall -Iinclude

# C Objects
OBJS =	obj/modes.o \
	obj/via65c22.o \
	obj/HD44870U.o \
	obj/acia65c51.o \
	obj/bus.o \
	obj/ram.o \
	obj/eeprom26c256.o \
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
	@mkdir -p obj
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
