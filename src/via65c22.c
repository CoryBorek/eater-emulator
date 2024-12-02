#include <via65c22.h>
#include <stdio.h>

unsigned char DDRA;
unsigned char DDRB;
unsigned char RA;
unsigned char RB;

void via_write(unsigned char reg, unsigned char data) {
    reg = reg & 0x0F;
    switch (reg) {
    case 0:
        RB = data & DDRB;
        break;
    case 1:
        RA = data & DDRA;
        break;
    case 2:
        DDRB = data;
        break;
    case 3:
        DDRA = data;
        break;
    default:
        break;
    }
}

unsigned char via_read(unsigned char reg) {
    reg = reg & 0x0F;
    switch (reg) {
    case 0b0000:
        return RB & ~DDRB & 0xFF;
        break;
    case 0b0001:
        return RA & ~DDRA & 0xFF;
        break;
    case 0b0010:
        return DDRB;
    case 0b0011:
        return DDRA;
        break;
    default:
        return 0;
        break;
    }
}


unsigned char out_a() {
    return RA & 0xFF;
}

unsigned char out_b() {
    return RB & 0xFF;
}

void in_a(unsigned char new_a) {
    RA = new_a & ~DDRA & 0xFF;
}

void in_b(unsigned char new_b) {
    RB = new_b & ~DDRB & 0xFF;
}
