#include <cpu/instr_1.h>
#include <cpu/addressing.h>
#include <cpu6502.h>
#include <string.h>

void clc() {
    strcpy(last_instr(), "CLC");
    impl();
    clockn(2);
    *C() = 0;
}
void instr1(unsigned char instr) {
    switch (instr) {
    case 0x8:
    default:    
        unknown();
        break;
    }
}
