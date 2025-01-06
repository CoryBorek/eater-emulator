#include <cpu/instr_8.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <string.h>

void sta_xind() {
    strcpy(last_instr(), "STA_XIND");
    ADDR addr;
    x_ind(&addr);
    str(A(), &addr);
}

void sty_zp() {
    strcpy(last_instr(), "STY_ZP");
    ADDR addr;
    zpg(&addr);
    str(Y(), &addr);
}

void sta_zp() {
    strcpy(last_instr(), "STA_ZP");
    ADDR addr;
    zpg(&addr);
    str(A(), &addr);
}

void stx_zp() {
    strcpy(last_instr(), "STX_ZP");
    ADDR addr;
    zpg(&addr);
    str(X(), &addr);
}

void dey() {
    strcpy(last_instr(), "DEY");
    der(Y());
}

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
    case 0x1:
        sta_xind();
        break;
    case 0x4:
        sty_zp();
        break;
    case 0x5:
        sta_zp();
        break;
    case 0x6:
        stx_zp();
        break;
    case 0x8:
        dey();
        break;
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
