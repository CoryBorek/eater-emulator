#include <cpu/instr_6.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <bus.h>
#include <cpu6502.h>

#include <string.h>

void rts() {
    strcpy(last_instr(), "RTS");
    clockn(6);
    (*sp())++;
    pc()->s.l = bus_read_data(0x100 + *sp());
    (*sp())++;
    pc()->s.h = bus_read_data(0x100 + *sp());
}

void pla() {
    strcpy(last_instr(), "PLA");
    pull(A());
}

void adc_imm() {
    strcpy(last_instr(), "ADC_IMM");
    unsigned char val;
    imm(&val);
    adc(val);
}

void rora() {
    strcpy(last_instr(), "RORA");
    impl();
    clockn(2);
    *A() = ror(*A());
}

void instr6(unsigned char instr) {
    switch (instr) {
    default:
        unknown();
        break;
    }
    
}
