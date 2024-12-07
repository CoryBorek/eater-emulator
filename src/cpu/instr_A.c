#include <cpu/instr_A.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <bus.h>
#include <cpu6502.h>
#include <string.h>
#include <stdio.h>

void ldy_imm() {
    strcpy(last_instr(), "LDY_IMM");
    unsigned char val;
    imm(&val);
    ldr(Y(), val);
}

void ldx_imm() {
    strcpy(last_instr(), "LDX_IMM");
    unsigned char val;
    imm(&val);
    ldr(X(), val);
}

void tay() {
    strcpy(last_instr(), "TAY");
    trr(A(), Y(), 1);
}

void lda_imm() {
    strcpy(last_instr(), "LDA_IMM");
    unsigned char val;
    imm(&val);
    ldr(A(), val);
}

void tax() {
    strcpy(last_instr(), "TAX");
    trr(A(), X(), 1);
}

void lda_abs() {
    strcpy(last_instr(), "LDA_ABS");
    ADDR addr;
    abs_a(&addr, 4);
    unsigned char val = bus_read_data(addr.p);
    ldr(A(), val);
}

void instrA(unsigned char instr) {
    
    switch (instr) {
    case 0x0:
        ldy_imm();
        break;
    case 0x2:
        ldx_imm();
        break;
    case 0x8:
        tay();
        break;
    case 0x9:
        lda_imm();
        break;
    case 0xA:
        tax();
        break;
    case 0xD:
        lda_abs();
        break;
    default:
        unknown();
        break;
    }
    
}
