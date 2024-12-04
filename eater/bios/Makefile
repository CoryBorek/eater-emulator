.PHONY: all cc65 clean

OBJS= bios.bin

all: cc65 $(OBJS)

cc65: bin/cc65

bin/cc65:
	git clone https://github.com/cc65/cc65.git
	$(MAKE) -C cc65 -j8
	mv cc65/bin .
	rm -rf cc65

bios.bin: bios.o
	./bin/ld65 -C bios.cfg -o $@ $< -Ln bios.sym

%.o: %.s
	./bin/ca65 $<
clean:
	rm -f $(OBJS) *.o
	rm -rf bin
