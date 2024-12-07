#include <cpu/instr_8.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <string.h>


void txa() {
    strcpy(last_instr(), "TXA");
    trr(X(), A(), 1);
}

void sty_abs() {
    strcpy(last_instr(), "STY_ABS");
    ADDR addr;
    abs_a(&addr, 1);
    str(Y(), &addr);
}

void sta_abs() {
    strcpy(last_instr(), "STA_ABS");
    ADDR addr;
    abs_a(&addr, 1);
    str(A(), &addr);
}

void instr8(unsigned char instr) {
    switch (instr) {
    case 0xA:
        txa();
        break;
    case 0xC:
        sty_abs();
        break;
    case 0xD:
        sta_abs();
        break;
    default:
        unknown();
        break;
    }

}
