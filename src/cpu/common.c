#include <cpu/common.h>
#include <cpu/addressing.h>
#include <cpu6502.h>
#include <bus.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void adc(unsigned char val) {
    unsigned short value = ((unsigned short)val);
    unsigned short temp = (unsigned short)*A() + value + (unsigned short)*C();
    *C() = (temp >> 8) & 0b1;
    *Z() = (temp & 0xFF) == 0 ? 1 : 0;
    *V() = (~((unsigned short)*A() ^ (unsigned short)value) & ((unsigned short)*A() ^ (unsigned short)temp) & 0x0080) ? 1 : 0;
    *N() = (temp >> 7) & 0b1;
    *A() = temp & 0x00FF;

}

void and(unsigned char val) {
    *A() = *A() & val;
    *Z() = *A() == 0 ? 1 : 0;
    *N() = ((*A() >> 7) & 0b1);
}

unsigned char asl(unsigned char val) {
    crash_me();
    return 0;
}

void bit(unsigned char val) {
    crash_me();
}

void branch(int check) {
    clockn(2);
    pc()->p++;
    unsigned char data = bus_read_data(pc()->p);
    pc()->p++;
    
    if (check) {
        ADDR pc_orig;
        pc_orig.p = pc()->p;
        pc()->p = (unsigned short)((short)pc()->p + (char)data);
        clock();
        if (pc_orig.s.h != pc()->s.h) {
            clock();
        }
    }
}

void cmp(unsigned char * reg, unsigned char val) {
    crash_me();
}

void dec(ADDR * addr) {
    crash_me();
}

void der(unsigned char * reg) {
    impl();
    clockn(2);
    (*reg)--;
    *N() = (*reg >> 7) & 0b1;
    *Z() = *reg == 0 ? 1 : 0;
}

void eor(unsigned char val) {
    crash_me();
}

void inc(ADDR * addr) {
    crash_me();
}

void inr(unsigned char * reg) {
    impl();
    clockn(2);
    (*reg)++;
    *N() = (*reg >> 7) & 0b1;
    *Z() = *reg == 0 ? 1 : 0;
}

void jmp(ADDR * addr) {
    pc()->p = addr->p;
}

void ldr(unsigned char * reg, unsigned char val) {
    *reg = val;
    *N() = (val >> 7 & 0b1);
    *Z() = val == 0 ? 1 : 0;
}

unsigned char lsr(unsigned char val) {
    crash_me();
    return 0;
}

void ora(ADDR * addr) {
    unsigned char val = bus_read_data(addr->p);
    *A() = *A() | val;
    *N() = (*A() >> 7) & 0b1;
    *Z() = *A() == 0 ? 1 : 0;
}

void push(unsigned char * reg) {
    impl();
    clockn(3);
    bus_write_data(0x100 + *sp(), *reg);
    (*sp())--;
}

void pull(unsigned char * reg) {
    impl();
    clockn(4);
    (*sp())++;
    unsigned char data = bus_read_data(0x100 + *sp());
    *reg = data;
    *N() = (data >> 7) & 0b1;
    *Z() = data == 0 ? 1 : 0;
}

unsigned char rol(unsigned char val) {
    unsigned short temp = (unsigned short)(val << 1) | *C();
    *C() = (temp >> 8) & 0b1;
    *Z() = (temp & 0xFF) == 0 ? 1 : 0;
    *N() = (temp >> 7) & 0b1;
    return (unsigned char)(temp & 0x00FF);
}

unsigned char ror(unsigned char val) {
    int temp = val & 0b1;
    val = val >> 1;
    val = (*C() << 7) + val;
    *C() = temp;
    *N() = (val >> 7) & 0b1;
    *Z() = val == 0 ? 1 : 0;
    return val;

}

void sbc(unsigned char val) {
    unsigned short value = ((unsigned short)val) ^ 0x00FF;
    unsigned short temp = (unsigned short)*A() + value + (unsigned short)*C();
    *C() = (temp & 0xFF00) ? 1 : 0;
    *Z() = (temp & 0xFF) == 0 ? 1 : 0;
    *V() = (temp ^ (unsigned short)*A()) & (temp ^ value) & 0x0080 ? 1 : 0;
    *N() = (temp >> 7) & 0b1;
    *A() = (unsigned char)(temp & 0x00FF);
}

void str(unsigned char * reg, ADDR * addr) {
    bus_write_data(addr->p, *reg);
}

void trr(unsigned char * reg1, unsigned char * reg2) {
    impl();
    clockn(2);
    *reg2 = *reg1;
    *N() = (*reg2 >> 7) & 0b1;
    *Z() = *reg2 == 0 ? 1 : 0;
}
