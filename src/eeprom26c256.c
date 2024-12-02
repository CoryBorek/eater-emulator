#include <eeprom26c256.h>
#include <bus.h>
#include <stdio.h>

int rom_begin;
int rom_size;

void eeprom_init(unsigned char * data, int start, int size) {
    rom_begin = start;
    rom_size = size;
    for (int i = start; i < start + size; i++) {
        bus_set_writeable(i, 1);
        bus_write_data(i, data[i - start]);
        bus_set_writeable(i, 0);
    }
}

void eeprom_deinit() {
    for (int i = rom_begin; i < rom_begin + rom_size; i++) {
        bus_set_writeable(i, 1);
        bus_write_data(i, 0xea);
        bus_set_writeable(i, 0);
    }
    rom_begin = 0;
    rom_size = 0; 
}
