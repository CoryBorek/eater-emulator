#ifndef EEPROM28C256_H
#define EEPROM28C256_H

void eeprom_init(unsigned char * data, int start, int size);

void eeprom_deinit();
#endif
