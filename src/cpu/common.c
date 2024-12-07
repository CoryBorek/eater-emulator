#include <cpu/common.h>
#include <cpu/addressing.h>
#include <cpu6502.h>
#include <bus.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void adc(unsigned char val) {
    unsigned char of = 0;
    if (((*A() >> 7) & 0b1) == ((val >> 7) & 0b1)) {
        of = 1;
    }

    unsigned short temp = (unsigned short)*A();
    temp += val;
    temp += (*C() & 0b1);

    *C() = (temp >> 8) & 0b1;
    *A() = (unsigned char)(temp & 0xFF);
    *N() = (temp >> 7) & 0b1;
    *Z() = temp == 0 ? 1 : 0;
    *V() = (of == 1 && ((val >> 7) & 0b1) != *N()) ? 1 : 0;
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
    *Z() = *A() & val == 0 ? 1 : 0;
    *N() = (val >> 7) & 0b1;
    *V() = (val >> 6) & 0b1;
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
    unsigned char val = bus_read_data(addr->p);
    val++;
    *N() = ((val >> 7) & 0b1) == 1 ? 1 : 0;
    *Z() = val == 0 ? 1 : 0;
    bus_write_data(addr->p, val);
}

void inr(unsigned char * reg) {
    impl();
    clockn(2);
    (*reg)++;
    *N() = ((*reg >> 7) & 0b1) == 1 ? 1 : 0;
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
    *reg = bus_read_data(0x100 + *sp());

    *N() = (*reg >> 7) & 0b1;
    *Z() = *reg == 0 ? 1 : 0;
}

unsigned char rol(unsigned char val) {
    unsigned char temp = (val >> 7) & 0b1;
    val = ((val << 1) & 0xFF) + *C();
    *C() = temp;
    *Z() = val == 0 ? 1 : 0;
    *N() = (val >> 7) & 0b1;
    return val;
}

unsigned char ror(unsigned char val) {
    unsigned char carry = val & 0b1;
    val = val >> 1;
    val = (*C() << 7) + val;
    *C() = carry;
    *N() = (((val >> 7) & 0b1) == 1) ? 1 : 0;
    *Z() = val == 0 ? 1 : 0;
    return val;
}

void sbc(unsigned char val) {
    
    unsigned char of = 0;
    if (((*A() >> 7) & 0b1) == ((val >> 7) & 0b1)) {
        of = 1;
    }
    short temp = (short)*A();
    temp += (*C() << 8);
    temp -= val;
    
    *C() = (temp >> 8) & 0b1;
    *A() = (unsigned char)(temp & 0xFF);
    *N() = (temp >> 7) & 0b1;
    *Z() = temp == 0 ? 1 : 0;
    *V() = (of == 1 && ((val >> 7) & 0b1) != *N()) ? 1 : 0;
}

void str(unsigned char * reg, ADDR * addr) {
    bus_write_data(addr->p, *reg);
}

void trr(unsigned char * reg1, unsigned char * reg2, int flag) {
    impl();
    clockn(2);
    *reg2 = *reg1;
    if (flag == 1) {
        *N() = (*reg2 >> 7) & 0b1;
        *Z() = *reg2 == 0 ? 1 : 0;
    }
}
