#ifndef ADDRESSING_H
#define ADDRESSING_H

typedef union {
    unsigned short p;
    unsigned char c[2];
    struct {
        unsigned char l;
        unsigned char h;
    } s;
} ADDR;

void crash_me();

void clockn(int n);

void abs_a(ADDR * addr, int extra);

void abs_x(ADDR * addr, int extra);

void abs_y(ADDR * addr, int extra);

void imm(unsigned char* val);

void impl();

void ind(ADDR * addr);

void x_ind(ADDR * addr);

void ind_y(ADDR * addr);

void rel(ADDR * addr);

void zpg(ADDR * addr);

void zpg_x(ADDR * addr);

void zpg_y(ADDR * addr);

#endif
