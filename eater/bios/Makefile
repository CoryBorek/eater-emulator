.PHONY: all cc65 clean

all: eater.bin

cc65: cc65/bin/cc65

cc65/bin/cc65:
	git clone https://github.com/cc65/cc65.git
	$(MAKE) -C cc65 -j8

msbasic/tmp/eater.bin: cc65/bin/cc65
	./msbasic.sh

eater.bin: msbasic/tmp/eater.bin
	cp msbasic/tmp/eater.bin .


clean:
	rm -rf cc65 msbasic eater.bin
