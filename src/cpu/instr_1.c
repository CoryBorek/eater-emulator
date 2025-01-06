#include <cpu/instr_1.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <string.h>


void bpl() {
    strcpy(last_instr(), "BPL");
    branch(*N() == 0);
}

void clc() {
    strcpy(last_instr(), "CLC");
    impl();
    clockn(2);
    *C() = 0;
}

void instr1(unsigned char instr) {
    switch (instr) {
    case 0x0:
        bpl();
        break;
    case 0x8:
        clc();
        break;
    default:    
        unknown();
        break;
    }
}
