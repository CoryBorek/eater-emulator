#include <cpu/instr_C.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>

void iny() {
    strcpy(last_instr(), "INY");
    inr(Y());
}
void dex() {
    strcpy(last_instr(), "DEX");
    der(X());
}

void instrC(unsigned char instr) {
    switch (instr) {
    case 0xA:
        dex();
        break;
    case 0x8:
        iny();
        break;
    default:
        unknown();
        break;
    }
    
}
