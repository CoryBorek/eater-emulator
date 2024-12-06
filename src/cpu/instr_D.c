#include <cpu/instr_D.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <string.h>

void bne() {
    strcpy(last_instr(), "BNE");
    branch(Z() == 0);
}

void instrD(unsigned char instr) {
    switch (instr) {
    default:
        unknown();
        break;
    }
}
