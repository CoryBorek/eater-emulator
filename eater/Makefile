ASARGS=-Fbin -dotdir

.PHONY: clean all

FILES ?= blink.bin rom.bin hello-world.bin hello-world-bad.bin number.bin hello-world-final.bin

all: vasm6502_oldstyle $(FILES)


%.bin: %.s
	./vasm6502_oldstyle $(ASARGS) -o $@ $<


rom.bin: makerom.py
	python makerom.py

vasm6502_oldstyle: make_vasm.sh
	bash ./make_vasm.sh

clean:
	rm -f $(FILES)
	rm -f vasm6502_oldstyle
