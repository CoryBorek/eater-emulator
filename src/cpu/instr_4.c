#include <cpu/instr_4.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>


void pha() {
    strcpy(last_instr(), "PHA");
    push(A());
}
void jmp_abs() {
    strcpy(last_instr(), "JMP_ABS");
    ADDR addr;
    abs_a(&addr, 0);
    jmp(&addr);
}

void instr4(unsigned char instr) {
    switch (instr) {
    case 0x8:
        pha();
        break;
    case 0xC:
        jmp_abs();
        break;
    default:
        unknown();
        break;
    }
}
