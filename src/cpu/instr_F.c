#include <cpu/instr_F.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>

void beq() {
    strcpy(last_instr(), "BEQ");
    branch(*Z() == 1);
}

void plx() {
    strcpy(last_instr(), "PLX");
    pull(X());
}

void instrF(unsigned char instr) {
    switch (instr) {
    case 0x0:
        beq();
        break;
    case 0xA:
        plx();
        break;
    default:
        unknown();
    }

}
