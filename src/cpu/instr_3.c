#include <cpu/instr_3.h>
#include <cpu/addressing.h>
#include <cpu6502.h>

#include <string.h>

void sec() {
    strcpy(last_instr(), "SEC");
    impl();
    clockn(2);
    *C() = 1;
}

void instr3(unsigned char instr) {
    switch (instr) {
    case 0x8:
        sec();
        break;
    default:
        unknown();
        break;
    }

}
