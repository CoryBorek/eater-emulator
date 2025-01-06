#include <cpu/instr_0.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <bus.h>

#include <string.h>

void ora_imm() {
    strcpy(last_instr(), "ORA_IMM");
    unsigned char val;
    imm(&val);
    ora(val);
}

void asl_a() {
    strcpy(last_instr(), "ASL_A");
    impl();
    clockn(2);
    *A() = asl(*A());
}

void ora_abs() {
    strcpy(last_instr(), "ORA_ABS");
    ADDR addr;
    abs_a(&addr, 0);
    unsigned char val = bus_read_data(addr.p);
    ora(val);
}

void instr0(unsigned char instr) {
    switch (instr) {
    case 0x9:
        ora_imm();
        break;
    case 0xA:
        asl_a();
        break;
    case 0xD:
        ora_abs();
        break;
    default:
        unknown();
    }
}
