#ifndef VIA65C22_H
#define VIA65C22_H

void via_write(unsigned char register, unsigned char data);

unsigned char via_read(unsigned char register);

unsigned char out_a();

unsigned char out_b();

void in_a(unsigned char new_a);

void in_b(unsigned char new_b);


#endif
