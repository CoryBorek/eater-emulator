#include <bus.h>

// TEMPORARY MEMORY;
unsigned char memory[65536];

void init_bus() {
    for (int i = 0; i < 65536; i++) {
        memory[i] = 0xea; // nop
    }
}

unsigned char bus_read_data(unsigned short addr) {
   return memory[addr];
}

void bus_write_data(unsigned short addr, unsigned char data) {
    memory[addr] = data;
}
