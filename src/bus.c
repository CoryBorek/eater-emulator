#include <bus.h>
#include <via65c22.h>
#include <stdio.h>
// TEMPORARY MEMORY;
unsigned char memory[0x10000];
char writeable[0x10000];

void init_bus() {
    for (int i = 0; i < 0x1000; i++) {
        memory[i] = 0xea; // nop
        writeable[i] = 0; // not writeable
    }
}

unsigned char bus_read_data(unsigned short addr) {
    if (addr >= 0x6000 && addr < 0x8000) {
        return via_read((unsigned char)(addr & 0xF));
    }
    return memory[addr];
}

void bus_write_data(unsigned short addr, unsigned char data) {
    if (addr >= 0x6000 && addr < 0x8000) {
        via_write((unsigned char)(addr & 0xF), data);
        return;
    }
    
    if (writeable[addr]) {
            memory[addr] = data;
    }
}

void bus_set_writeable(unsigned short addr, char data) {
    writeable[(int)addr] = data;
}
