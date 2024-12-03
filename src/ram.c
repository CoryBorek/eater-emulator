#include <ram.h>
#include <bus.h>

unsigned short ram_start;
unsigned short ram_size;
void init_ram(unsigned short start, unsigned short size) {
    ram_start = start;
    ram_size = size;
    for (int i = start; i < start + size; i++) {
        bus_set_writeable(i, 1);
    }
}

void deinit_ram() {
    for (int i = ram_start; i < ram_start + ram_size; i++) {
        bus_set_writeable(i, 0);
    }
    ram_start = 0;
    ram_size = 0;
}
