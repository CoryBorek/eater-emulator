#include <cpu/instr_7.h>
#include <cpu/addressing.h>
#include <cpu6502.h>

#include <string.h>

void sei() {
    strcpy(last_instr(), "SEI");
    impl();
    clockn(2);
    *I() = 1;
}

void instr7(unsigned char instr) {
    switch (instr) {
    default:
         unknown();
         break;
    }
}
