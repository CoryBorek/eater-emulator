#include <cpu6502.h>
#include <stdio.h>
#include <unistd.h>
#include <bus.h>

// accumulator, x and y registers/
unsigned char A, X, Y;

// program counter
typedef union {
    unsigned short p;
    unsigned char c[2];
    struct {
        unsigned char l;
        unsigned char h;
    } s;
} ADDR;
ADDR pc;

// Status register
unsigned char N, V, B, D, O, Z, C;

void abs_a(ADDR* addr, int extra) {
    unsigned char h_i = pc.s.h;
    clock();
    pc.p++;
    addr->c[0] = bus_read_data(pc.p);
    clock();
    pc.p++;
    addr->c[1] = bus_read_data(pc.p);
    clock();
    pc.p++;
    unsigned char h_f = pc.s.h;

    if (h_f != h_i || extra) {
        printf("page different\n");
        clock();
    }
}

void NOP() {
    clock();
    clock();
    pc.p++;
    printf("NOP\n");
}

void interrupt() {

}

void non_maskable_interrupt() {

}

void clock() {
    usleep(1);
}

void reset() {
    for (int i = 0; i < 7; i++) {
        clock();
    }
    // set status flags to zero.
    N = 0;
    V = 0;
    B = 0;
    D = 0;
    O = 0;
    Z = 0;
    C = 0;

    // set registers to zero.
    A = 0;
    X = 0;
    Y = 0;

    // initialize data pointer.
    pc.c[0] = bus_read_data(0xFFFC);
    clock();
    pc.c[1] = bus_read_data(0xFFFD);
    clock();
}

void run_instr() {
    unsigned char instr = bus_read_data(pc.p);
    switch (instr) {
    case 0xEA:
    default:
        NOP();
        break;
    }
}
