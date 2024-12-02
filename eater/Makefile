ASARGS=-Fbin -dotdir

.PHONY: clean all

all: blink.bin rom.bin hello-world.bin

hello-world.bin: hello-world.s vasm6502_oldstyle
	./vasm6502_oldstyle $(ASARGS) hello-world.s
	mv a.out hello-world.bin
blink.bin: blink.s vasm6502_oldstyle
	./vasm6502_oldstyle $(ASARGS) blink.s
	mv a.out blink.bin

rom.bin: makerom.py
	python makerom.py

vasm6502_oldstyle: make_vasm.sh
	bash ./make_vasm.sh

clean:
	rm -f blink.bin hello-world.bin
	rm -f vasm6502_oldstyle
	rm -f rom.bin
