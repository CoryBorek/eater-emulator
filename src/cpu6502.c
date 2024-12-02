#include <cpu6502.h>
#include <stdio.h>
#include <unistd.h>
#include <bus.h>
#include <stdlib.h>
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
        //printf("page different\n");
        clock();
    }
}

void imm(unsigned char* val) {
    clock();
    pc.p++;
    *val = bus_read_data(pc.p);
    clock();
    pc.p++;
}

void JMP_A() {
    ADDR addr;
    abs_a(&addr, 0);
    pc.p = addr.p;
    //printf("JMP $%x\n", addr.p);
}

void RORA() {
    unsigned char carry = A & 0b1;
    A = A >> 1;
    A = (C << 7) + A;
    C = carry;
    //printf("ROR (A) %x\n", A);
    clock();
    clock();
    pc.p++;
}

void STA_A() {
    ADDR addr;
    abs_a(&addr, 1);
    bus_write_data(addr.p, A);
    //printf("STA $%x\n", addr.p);
}

void LDA_IMM() {
    unsigned char val;
    imm(&val);
    A = val;
    //printf("LDA #(%x)\n", val);
}



void NOP() {
    clock();
    clock();
    pc.p++;
    //printf("NOP\n");
}

void interrupt() {

}

void non_maskable_interrupt() {

}

void clock() {
    //usleep(1);
    usleep(10000);
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
    case 0x4C:
        JMP_A();
        break;
    case 0x6A:
        RORA();
        break;
    case 0x8D:
        STA_A();
        break;
    case 0xA9:
        LDA_IMM();
        break;
    case 0xEA:
        NOP();
        break;
    default:
        printf("Unknown instruction: %x. Exiting.", instr);
        exit(1);
        break;
    }
}
