#include <cpu/instr_0.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>
void ora_abs() {
    strcpy(last_instr(), "ORA_ABS");
    ADDR addr;
    abs_a(&addr, 0);
    ora(&addr);
}

void instr0(unsigned char instr) {
    
    switch (instr) {
    default:
        unknown();
    }
}
