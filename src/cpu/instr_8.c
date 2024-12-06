#include <cpu/instr_8.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <string.h>


void txa() {
    strcpy(last_instr(), "TXA");
    trr(X(), A());
}

void sta_abs() {
    strcpy(last_instr(), "STA_ABS");
    ADDR addr;
    abs_a(&addr, 1);
    str(A(), &addr);
}

void instr8(unsigned char instr) {
    switch (instr) {
    default:
        unknown();
        break;
    }

}
