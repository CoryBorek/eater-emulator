#include <cpu/instr_2.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <bus.h>
#include <cpu6502.h>

#include <string.h>
#include <stdio.h> // delete later
void jsr() {
    strcpy(last_instr(), "JSR");
    ADDR addr;
    abs_a(&addr, 3);
    bus_write_data(0x100 + *sp(), pc()->c[1]);
    (*sp())--;
    bus_write_data(0x100 + *sp(), pc()->c[0]);
    (*sp())--;
    pc()->p = addr.p;
}

void and_imm() {
    strcpy(last_instr(), "AND_IMM");
    unsigned char val;
    imm(&val);
    and(val);
}

void rol_abs() {
    strcpy(last_instr(), "ROL_ABS");
    ADDR addr;
    abs_a(&addr, 3);
    unsigned char val = bus_read_data(addr.p);
    val = rol(val);
    bus_write_data(addr.p, val);
}

void instr2(unsigned char instr) {
    switch (instr) {
    case 0x0:
        jsr();
        break;
    case 0x9:
        and_imm();
        break;
    case 0xE:
        rol_abs();
        break;
    default:
        unknown();
        break;
    }
}
