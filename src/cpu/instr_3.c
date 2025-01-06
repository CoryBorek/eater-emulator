#include <cpu/instr_3.h>
#include <cpu/addressing.h>
#include <cpu/common.h>
#include <cpu6502.h>

#include <string.h>


void bmi() {
    strcpy(last_instr(), "BMI");
    branch(*N() == 1);
}

void sec() {
    strcpy(last_instr(), "SEC");
    impl();
    clockn(2);
    *C() = 1;
}

void dec_a() {
    strcpy(last_instr(), "DEC");
    der(A());
}

void instr3(unsigned char instr) {
    switch (instr) {
    case 0x0:
        bmi();
        break;
    case 0x8:
        sec();
        break;
    case 0xA:
        dec_a();
        break;
    default:
        unknown();
        break;
    }

}
