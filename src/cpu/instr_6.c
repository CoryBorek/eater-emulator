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
    pc()->c[0] = bus_read_data(0x100 + *sp());
    (*sp())++;
    pc()->c[1] = bus_read_data(0x100 + *sp());
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
    case 0x0:
        rts();
        break;
    case 0x8:
        pla();
        break;
    case 0x9:
        adc_imm();
        break;
    case 0xA:
        rora();
        break;
    default:
        unknown();
        break;
    }
    
}
