#include <cpu/instr_E.h>
#include <cpu/addressing.h>
#include <cpu6502.h>
#include <cpu/common.h>

#include <string.h>

void inx() {
    strcpy(last_instr(), "INX");
    inr(X());
}

void sbc_imm() {
    strcpy(last_instr(), "SBC_IMM");
    unsigned char val;
    imm(&val);
    sbc(val);
}
void instrE(unsigned char instr) {
    switch(instr) {
    default:
        unknown();
    }
}
