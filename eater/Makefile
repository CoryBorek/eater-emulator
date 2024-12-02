
.PHONY: clean all

all: blink.bin rom.bin


blink.bin: blink.s vasm6502_oldstyle
	./vasm6502_oldstyle -Fbin -dotdir blink.s
	mv a.out blink.bin

rom.bin: makerom.py
	python makerom.py

vasm6502_oldstyle: make_vasm.sh
	bash ./make_vasm.sh

clean:
	rm -f blink.bin
	rm -f vasm6502_oldstyle
	rm -f rom.bin
