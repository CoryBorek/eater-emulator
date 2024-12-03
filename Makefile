
TARGET_DIR ?= .
CC? = gcc
CARGS= -Wall -Iinclude
OBJS =	obj/via65c22.o \
	obj/HD44870U.o \
	obj/bus.o \
	obj/ram.o \
	obj/eeprom26c256.o \
	obj/cpu6502.o

.PHONY: all clean

all: $(TARGET_DIR)/emu

$(TARGET_DIR)/emu: src/main.c $(OBJS)
	@$(CC) $(CARGS) -o $@ $(OBJS) src/main.c
	@echo Created $@

obj/%.o: src/%.c
	@mkdir -p obj
	@$(CC) $(CARGS) -c -o $@ $<
	@echo CC    $<

clean:
	@rm -rf obj
	@rm -rf $(TARGET_DIR)/emu
	@echo Cleaned!
