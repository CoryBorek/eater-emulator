#include <cpu/addressing.h>
#include <bus.h>
#include <cpu6502.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void crash_me() {
    traceback();
    exit(0);
}

void clockn(int n) {
    for (int i = 0; i < n; i++) {
        clock();
    }
}

void pp() {
    pc()->p++;
}


void abs_a(ADDR * addr, int extra) {
    pp();
    addr->c[0] = bus_read_data(pc()->p);
    pp();
    addr->c[1] = bus_read_data(pc()->p);
    pp();
    clockn(3 + extra);
}

void abs_x(ADDR * addr, int extra) {
    ADDR temp;
    temp.p = addr->p;
    pp();
    addr->c[0] = bus_read_data(pc()->p);
    pp();
    addr->c[1] = bus_read_data(pc()->p);
    pp();
    addr->p += (char)(*X());
    
    if (extra < 0) {
        if (temp.s.h != addr->s.h) {
            extra = 0 - extra;
            clock();
            extra--;
        }
    }
    clockn(4 + extra);
}

void abs_y(ADDR * addr, int extra) {
    ADDR temp;
    temp.p = addr->p;
    pp();
    addr->c[0] = bus_read_data(pc()->p);
    pp();
    addr->c[1] = bus_read_data(pc()->p);
    pp();
    addr->p += (char)(*Y());
    
    if (extra < 0) {
        if (temp.s.h != addr->s.h) {
            extra = 0 - extra;
            clock();
            extra--;
        }
    }
    clockn(4 + extra);
}

void imm(unsigned char* val) {
    clockn(2);
    pp();
    *val = bus_read_data(pc()->p);
    pp();
}

void impl() {
    pp();
}

void ind(ADDR * addr) {
    clockn(5);
    pp();
    ADDR addr2;
    addr2.c[0] = bus_read_data(pc()->p);
    pp();
    addr2.c[1] = bus_read_data(pc()->p);
    pp();
    addr->c[0] = bus_read_data(addr2.p);
    addr->c[1] = bus_read_data(addr2.p + 1);
    
}

void x_ind(ADDR * addr) {
    clockn(6);
    pp();
    ADDR addr2;
    addr2.c[0] = bus_read_data(pc()->p);
    addr2.c[1] = 0x00;
    addr2.c[0] += *X();
    pp();
    addr->c[0] = bus_read_data(addr2.p);
    addr->c[1] = bus_read_data(addr2.p + 1);
    
}

void ind_y(ADDR * addr) {
    crash_me();
}

void rel(ADDR * addr) {
    crash_me();
}

void zpg(ADDR * addr) {
    clockn(3);
    pp();
    addr->c[0] = bus_read_data(pc()->p);
    addr->c[1] = 0x00;
    pp();
    
}

void zpg_x(ADDR * addr) {
    clockn(4);
    pp();
    addr->c[0] = bus_read_data(pc()->p);
    addr->c[1] = 0x00;
    addr->p += *X();
    pp();
}

void zpg_y(ADDR * addr) {
    crash_me();
}
