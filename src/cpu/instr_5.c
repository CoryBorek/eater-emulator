#include <cpu/instr_5.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>

void cli() {
    strcpy(last_instr(), "CLI");
    impl();
    clockn(2);
    *I() = 0;
}

void instr5(unsigned char instr) {
    switch (instr) {
    case 0x8:
        cli();
        break;
    default:
        unknown();
        break;
    }
}
