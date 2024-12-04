#include <via65c22.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpu6502.h>
#include <modes.h>

unsigned char DDRA;
unsigned char DDRB;
unsigned char RA;
unsigned char RB;

unsigned char ier;
unsigned char ifr;
unsigned char pcr;

void via_write(unsigned char reg, unsigned char data) {
    reg = reg & 0x0F;
    switch (reg) {
    case 0x0:
        RB = data & DDRB;
        break;
    case 0x1:
        RA = data & DDRA;
        if ((ifr & 0b10000010) == 0b10000010) {
            ifr &= 0b01111101;
            set_irq_available(1);
        }
        break;
    case 0x2:
        DDRB = data;
        break;
    case 0x3:
        DDRA = data;
        break;
    case 0xC:
        pcr = data;
    case 0x0D:
        ifr = data;
        break;
    case 0xE:
        unsigned char set_clear = (data >> 7) & 0b1;
        if (set_clear == 1) {
            ier |= data;
        } else {
            ier &= ~data;
         }
        break;
    default:
        printf("Unknown VIA write: %d, %x\n", reg, data);
        exit(0);
        break;
    }
}

unsigned char via_read(unsigned char reg) {
    reg = reg & 0x0F;
    switch (reg) {
    case 0x0:
        return RB & ~DDRB & 0xFF;
        break;
    case 0x1:
        if ((ifr & 0b10000010) == 0b10000010) {
            ifr &= 0b01111101;
            set_irq_available(1);
        }
        return RA & ~DDRA & 0xFF;
        break;
    case 0x2:
        return DDRB;
    case 0x3:
        return DDRA;
        break;
    case 0xC:
        return pcr;
        break;
    case 0xD:
        return ifr;
        break;
    case 0xE:
        return ier;
        break;
    default:
        printf("Unknown VIA read: %d\n", reg);
        exit(0);
        break;
    }
    return 0;
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


void ca1(int val) {
    unsigned char ctrl = pcr & 0b1;
    if ((val & 0b1) == ctrl) {
        if ((ier & 0b00000010) > 0) {
            ifr |= 0b10000010;
            interrupt();
        }
    }
}
