#include <cpu/instr_E.h>
#include <bus.h>
#include <cpu/addressing.h>
#include <cpu6502.h>
#include <cpu/common.h>

#include <string.h>

void sbc_zp() {
    strcpy(last_instr(), "SBC_ZP");
    ADDR addr;
    zpg(&addr);
    unsigned char val = bus_read_data(addr.p);
    sbc(val);
}

void inc_zp() {
    strcpy(last_instr(), "INC_ZP");
    ADDR addr;
    zpg(&addr);
    clockn(2);
    inc(&addr);
}

void inx() {
    strcpy(last_instr(), "INX");
    inr(X());
}

void sbc_imm() {
    strcpy(last_instr(), "SBC_IMM");
    unsigned char val;
    imm(&val);
    sbc(val);
}

void inc_abs() {
    strcpy(last_instr(), "INC_ABS");
    ADDR addr;
    abs_a(&addr, 3);
    inc(&addr);
}

void instrE(unsigned char instr) {
    switch(instr) {
    case 0x5:
        sbc_zp();
        break;
    case 0x6:
        inc_zp();
        break;
    case 0x8:
        inx();
        break;
    case 0x9:
        sbc_imm();
        break;
    case 0xE:
        inc_abs();
        break;
    default:
        unknown();
    }
}
