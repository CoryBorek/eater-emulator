#include <cpu6502.h>
#include <stdio.h>
#include <unistd.h>
#include <bus.h>
#include <stdlib.h>

//#define DEBUG
// accumulator, x and y registers/
unsigned char A, X, Y;

// stack pointer
unsigned char sp;

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

void JSR() {
     ADDR addr;
     abs_a(&addr, 1);
     
     bus_write_data(0x0100 + sp, pc.c[1]);
     sp--;
     bus_write_data(0x0100 + sp, (pc.c[0]));
     sp--;
#ifdef DEBUG
     printf("JSR %x\n", addr.p);
#endif
     pc.p = addr.p;
     clock();
     clock();
}

void ANDI() {
    unsigned char val;
    imm(&val);
    A = A & val;
    Z = A == 0 ? 1 : 0;
    N = ((A >> 7) & 0b1) == 1 ? 1 : 0; 
#ifdef DEBUG
   printf("AND %x\n", A);
#endif

}


void PHA() {
    bus_write_data(0x100 + sp, A);
    sp--;
#ifdef DEBUG
    printf("PHA %x\n", A);
#endif
    clock();
    clock();
    clock();
    pc.p++;
}

void RTS() {
    sp++;
    pc.c[0] = bus_read_data(0x0100 + sp);
    sp++;
    pc.c[1] = bus_read_data(0x0100 + sp);
    for (int i = 0; i < 6; i++) {
        clock();    
    }
#ifdef DEBUG
     printf("RTS\n");
#endif
}
void JMP_A() {
    ADDR addr;
    abs_a(&addr, 0);
    pc.p = addr.p;
#ifdef DEBUG
     printf("JMP $%x\n", addr.p);
#endif
}

void PLA() {
    sp++;
    A = bus_read_data(0x100 + sp);
    
#ifdef DEBUG
    printf("PLA %x\n", A);
#endif
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    
    clock();
    clock();
    clock();
    clock();
    pc.p++;
}
void RORA() {
    unsigned char carry = A & 0b1;
    A = A >> 1;
    A = (C << 7) + A;
    C = carry;
#ifdef DEBUG
    printf("ROR (A) %x\n", A);
#endif
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;
}

void STA_A() {
    ADDR addr;
    abs_a(&addr, 1);
    bus_write_data(addr.p, A);
    
#ifdef DEBUG
    printf("STA $%x\n", addr.p);
#endif
}

void LDA_IMM() {
    unsigned char val;
    imm(&val);
    A = val;
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
#ifdef DEBUG
   printf("LDA #(%x)\n", val);
#endif

}

void LDA_A() {
    ADDR addr;
    abs_a(&addr, 1);
    A = bus_read_data(addr.p);    
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
#ifdef DEBUG
   printf("LDA $(%x)\n", addr.p);
#endif

}

void BNE() {
    clock();
    clock();
    pc.p++;
    unsigned char data;
    data = bus_read_data(pc.p);
    pc.p++;
    
    //printf("Z: %d\n", Z);
    if (Z == 0) {
        ADDR pc_orig;
        pc_orig.p = pc.p;
        pc.p = (unsigned short)((short)pc.p + (char) data);
        clock();
        if (pc_orig.s.h != pc.s.h) {
            clock();
        }
    }

#ifdef DEBUG
    printf("BNE %d: %d\n", (char)data, Z);
#endif
}

void NOP() {
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
   printf("NOP\n");
#endif
}

void BEQ() {
    clock();
    clock();
    pc.p++;
    unsigned char data;
    data = bus_read_data(pc.p);
    pc.p++;

    if (Z == 1) {
        ADDR pc_orig;
        pc_orig.p = pc.p;
        pc.p = (unsigned short)(short)pc.p + ((char) data);
        clock();
        if (pc_orig.s.h != pc.s.h) {
            clock();
        }
    }

#ifdef DEBUG
    printf("BEQ %x: %d\n", data, Z);
#endif
}

void interrupt() {

}

void non_maskable_interrupt() {

}

void clock() {
    usleep(1);
    //usleep(10000);
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
    
    // set stack pointer
    sp = 0xFD;
    // initialize data pointer.
    pc.c[0] = bus_read_data(0xFFFC);
    clock();
    pc.c[1] = bus_read_data(0xFFFD);
    clock();
}

void run_instr() {
    unsigned char instr = bus_read_data(pc.p);
    switch (instr) {
    case 0x20:
        JSR();
        break;
    case 0x29:
        ANDI();
        break;
    case 0x48:
        PHA();
        break;
    case 0x4C:
        JMP_A();
        break;
    case 0x60:
        RTS();
        break;
    case 0x68:
        PLA();
        break;
    case 0x6A:
        RORA();
        break;
    case 0x8D:
        STA_A();
        break;
    case 0xAD:
        LDA_A();
        break;
    case 0xA9:
        LDA_IMM();
        break;
    case 0xD0:
        BNE();
        break;
    case 0xEA:
        NOP();
        break;
    case 0xF0:
        BEQ();
        break;
    default:
        printf("Unknown instruction: %x at %x. Exiting.", instr, pc.p);
        exit(1);
        break;
    }
}
