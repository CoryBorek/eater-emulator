#include <cpu/instr_C.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <bus.h>

#include <string.h>

void cpy_zp() {
    strcpy(last_instr(), "CPY_ZPG");
    ADDR addr;
    zpg(&addr);
    unsigned char val = bus_read_data(addr.p);
    cmp(Y(), val);
}

void iny() {
    strcpy(last_instr(), "INY");
    inr(Y());
}

void cmp_imm() {
    strcpy(last_instr(), "CMP_IMM");
    unsigned char val;
    imm(&val);
    cmp(A(), val);
}

void dex() {
    strcpy(last_instr(), "DEX");
    der(X());
}

void instrC(unsigned char instr) {
    switch (instr) {
    case 0x4:
        cpy_zp();
        break;
    case 0x8:
        iny();
        break;
    case 0x9:
        cmp_imm();
        break;
    case 0xA:
        dex();
        break;
    default:
        unknown();
        break;
    }
    
}
