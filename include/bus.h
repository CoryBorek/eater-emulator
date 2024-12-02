#ifndef BUS_H
#define BUS_H

void init_bus();

unsigned char bus_read_data(unsigned short addr);

void bus_write_data(unsigned short addr, unsigned char data);
#endif
