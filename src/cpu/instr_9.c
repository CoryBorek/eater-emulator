#include <cpu/instr_9.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>

void bcc() {
    strcpy(last_instr(), "BCC");
    branch(*C() == 0);
}

void sta_y() {
    strcpy(last_instr(), "STA_Y");
    ADDR addr;
    abs_y(&addr, 1);
    str(A(), &addr);
}

void txs() {
    strcpy(last_instr(), "TXS");
    trr(X(), sp());
}

void instr9(unsigned char instr) {
    switch (instr) {
    default:        
        unknown();  
        break;
    }
}
