#ifndef CPU_COMMON_H
#define CPU_COMMON_H
#include <cpu/addressing.h>

void adc(unsigned char val);

void and(unsigned char val);

unsigned char asl(unsigned char val);

void bit(unsigned char val);

void branch(int check);

void cmp(unsigned char * reg, unsigned char val);

void dec(ADDR * addr);

void der(unsigned char * reg);

void eor(unsigned char val);

void inc(ADDR * addr);

void inr(unsigned char * reg);

void jmp(ADDR * addr);

void ldr(unsigned char * reg, unsigned char val);

unsigned char lsr(unsigned char val);

void ora(ADDR * addr);

void push(unsigned char * reg);

void pull(unsigned char * reg);

unsigned char rol(unsigned char val);

unsigned char ror(unsigned char val);

void sbc(unsigned char val);

void str(unsigned char * reg, ADDR * addr);

void trr(unsigned char * reg1, unsigned char * reg2);

#endif
