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
    default:
        unknown();
        break;
    }
    
}
