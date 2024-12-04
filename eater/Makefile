
.PHONY: all clean pre-bios bios


all: pre-bios bios


pre-bios:
	@$(MAKE) -C pre-bios

bios:
	@$(MAKE) -C bios

clean:
	@$(MAKE) -C pre-bios clean
	@$(MAKE) -C bios clean
