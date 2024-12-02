
TARGET_DIR ?= .
CC? = gcc
CARGS= -Wall -Iinclude
OBJS = obj/main.o

.PHONY: all clean

all: main

$(TARGET_DIR)/main: $(OBJS)
	@$(CC) $(CARGS) -o $(TARGET_DIR)/main $<
	@echo Created $(TARGET_DIR)/main

obj/%.o: src/%.c
	@mkdir -p obj
	@$(CC) $(CARGS) -c -o $@ $<
	@echo CC    $<

clean:
	@rm -rf obj
	@rm -rf $(TARGET_DIR)/main
	@echo Cleaned!
