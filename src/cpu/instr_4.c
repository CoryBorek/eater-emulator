#include <cpu/instr_4.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>
#include <bus.h>
#include <string.h>


void rti() {
    strcpy(last_instr(), "RTI");
    impl();
    clockn(6);
    (*sp())++;
    unsigned char sr_data = bus_read_data(0x100 + *sp());
    set_status_register(sr_data);
    (*sp())++;
    pc()->c[0] = bus_read_data(0x100 + *sp());
    (*sp())++;
    pc()->c[1] = bus_read_data(0x100 + *sp());

}

void pha() {
    strcpy(last_instr(), "PHA");
    push(A());
}

void jmp_abs() {
    strcpy(last_instr(), "JMP_ABS");
    ADDR addr;
    abs_a(&addr, 0);
    jmp(&addr);
}

void instr4(unsigned char instr) {
    switch (instr) {
    case 0x0:
        rti();
        break;
    case 0x8:
        pha();
        break;
    case 0xC:
        jmp_abs();
        break;
    default:
        unknown();
        break;
    }
}
