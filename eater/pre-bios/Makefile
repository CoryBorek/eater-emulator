#
# Assembly Arguments
#
ASARGS=-Fbin -dotdir -wdc02

#
# Phony arguments
#
.PHONY: clean all

#
# All Assembly files
#
FILES ?= blink.bin \
	rom.bin \
	hello-world.bin \
	hello-world-bad.bin \
	number.bin \
	hello-world-final.bin \
	interrupt.bin \
	rs232.bin \
	wozmon.bin

#
# Compile everything...
#
all: vasm6502_oldstyle $(FILES)

#
# Compile Assembly files
#
%.bin: %.s
	./vasm6502_oldstyle $(ASARGS) -o $@ $< > /dev/null
	@ echo AS    $@

#
# Make rom.bin from python.
#
rom.bin: makerom.py
	@python makerom.py
	@echo made rom.bin

#
# Build the assembler
#
vasm6502_oldstyle: make_vasm.sh
	@bash ./make_vasm.sh > /dev/null
	@echo Downloaded and compiled VASM.

#
# Clean up the program
#
clean:
	@rm -f $(FILES)
	@rm -f vasm6502_oldstyle
	@echo Cleaned up.
