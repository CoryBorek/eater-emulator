#include <cpu/instr_C.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>

void iny() {
    strcpy(last_instr(), "INY");
    inr(Y());
}

void cmp_imm() {
    strcpy(last_instr(), "CMP_IMM");
    unsigned char val;
    imm(&val);
    cmp(A(), val);
}

void dex() {
    strcpy(last_instr(), "DEX");
    der(X());
}

void instrC(unsigned char instr) {
    switch (instr) {
    case 0x8:
        iny();
        break;
    case 0x9:
        cmp_imm();
        break;
    case 0xA:
        dex();
        break;
    default:
        unknown();
        break;
    }
    
}
