#include <cpu/instr_B.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <bus.h>
#include <cpu6502.h>

#include <string.h>

void lda_ax() {
    strcpy(last_instr(), "LDA_ABSX");
    ADDR addr;
    abs_x(&addr, -1);
    unsigned char val = bus_read_data(addr.p);
    ldr(A(), val);
}


void lda_ay() {
    strcpy(last_instr(), "LDA_ABSY");
    ADDR addr;
    abs_y(&addr, -1);
    unsigned char val = bus_read_data(addr.p);
    ldr(A(), val);
}

void instrB(unsigned char instr) {
    switch (instr) {
    default:
        unknown();
        break;
    }

}
