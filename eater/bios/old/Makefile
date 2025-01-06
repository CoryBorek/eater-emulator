.PHONY: all cc65 clean

all: cc65 bios.bin

cc65: cc65/bin/cc65

cc65/bin/cc65:
	git clone https://github.com/cc65/cc65.git
	$(MAKE) -C cc65 -j8

bios.bin: bios.o 
	./cc65/bin/ld65 -C bios.cfg -o $@ $< -Ln bios.lbl

bios.o: bios.s wozmon.s
	./cc65/bin/ca65 bios.s
clean:
	rm -f bios.bin *.o bios.lbl
	rm -rf cc65
