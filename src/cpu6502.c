#include <cpu6502.h>
#include <stdio.h>
#include <unistd.h>
#include <bus.h>
#include <stdlib.h>
#include <string.h>
#include <modes.h>

#include <cpu/addressing.h>
#include <cpu/instr_0.h>
#include <cpu/instr_1.h>
#include <cpu/instr_2.h>
#include <cpu/instr_3.h>
#include <cpu/instr_4.h>
#include <cpu/instr_5.h>
#include <cpu/instr_6.h>
#include <cpu/instr_7.h>
#include <cpu/instr_8.h>
#include <cpu/instr_9.h>
#include <cpu/instr_A.h>
#include <cpu/instr_B.h>
#include <cpu/instr_C.h>
#include <cpu/instr_D.h>
#include <cpu/instr_E.h>
#include <cpu/instr_F.h>

// accumulator, x and y registers/
unsigned char AR, XR, YR;

// stack pointer
unsigned char SPR;

// Status register
unsigned char NS, VS, BS, DS, IS, ZS, CS;

// program counter
ADDR program_counter;

// utility flags
char last_instr_str[16] = {0}; 
int in_irq_val = 0;

/**
 * Starts a new interrupt
 */
void start_interrupt() {
    bus_write_data(0x0100 + SPR, program_counter.c[1]);
    SPR--;
    bus_write_data(0x0100 + SPR, (program_counter.c[0]));
    SPR--;
    
    unsigned char sr = status_register();
    bus_write_data(0x100 + SPR, sr);
    SPR--;
    
    for (int i = 0; i < 7; i++) {
        clock();
    }
    
}

void interrupt() {
    if (IS != 0) {
        return;
    }
    strcpy(last_instr_str, "INTERRUPT");

    if (get_irq_mode() == 0) {
        set_irq_available(0);
        in_irq_val = 1;
    }

    start_interrupt();
    program_counter.c[0] = bus_read_data(0xFFFE);
    program_counter.c[1] = bus_read_data(0xFFFF);
}


void non_maskable_interrupt() {
    strcpy(last_instr_str, "NM INTERRUPT");
    start_interrupt();
    program_counter.c[0] = bus_read_data(0xFFFA);
    program_counter.c[1] = bus_read_data(0xFFFB);
}

void run_instr() {
    unsigned char instr_full = bus_read_data(program_counter.p);
    unsigned char instr_hi = (instr_full >> 4) & 0xF;
    unsigned char instr = (instr_full & 0xF);
    switch (instr_hi) {
    case 0x0:
        instr0(instr);
        break;
    case 0x1:
        instr1(instr);
        break;
    case 0x2:
        instr2(instr);
        break;
    case 0x3:
        instr3(instr);
        break;
    case 0x4:
        instr4(instr);
        break;
    case 0x5:
        instr5(instr);
        break;
    case 0x6:
        instr6(instr);
        break;
    case 0x7:
        instr7(instr);
        break;
    case 0x8:
        instr8(instr);
        break;
    case 0x9:
        instr9(instr);
        break;
    case 0xA:
        instrA(instr);
        break;
    case 0xB:
        instrB(instr);
        break;
    case 0xC:
        instrC(instr);
        break;
    case 0xD:
        instrD(instr);
        break;
    case 0xE:
        instrE(instr);
        break;
    case 0xF:
        instrF(instr);
        break;
    default:
        unknown();
        break;
    }
    
#ifdef DEBUG
    printf("INSTR: %s\n", last_instr_str);
#endif
}

void unknown() {
    printf("ERROR\n");
    strcpy(last_instr_str, "ERROR");
#ifndef NCURSES
    traceback();
    exit(1);
#endif
}

void clock() {
    int useconds = get_clock_speed();
    usleep(useconds);

}

void reset() {
    for (int i = 0; i < 7; i++) {
        clock();
    }
    // set status flags to zero.
    NS = 0;
    VS = 0;
    BS = 0;
    DS = 0;
    IS = 1;
    ZS = 0;
    CS = 0;

    // set registers to zero.
    AR = 0;
    XR = 0;
    YR = 0;
    
    // set stack pointer
    SPR = 0xFD;
    // initialize data pointer.
    program_counter.c[0] = bus_read_data(0xFFFC);
    clock();
    program_counter.c[1] = bus_read_data(0xFFFD);
    clock();
    strcpy(last_instr_str, "RESET");
}

void traceback() {
    unsigned char instr = bus_read_data(program_counter.p);
    printf("DEBUG DATA: A: %x:%d, X: %X:%d, Y: %X:%d, C: %d, N: %d, Z: %d, V: %d\n",
         AR, AR, XR, XR, YR, YR, (int)CS, (int)NS, (int)ZS, (int)VS);
    printf("SP: %x\n\n", SPR);
    printf("instruction: %x at %x. Exiting.\n", instr, program_counter.p);
    printf("last instruction string: %s\n", last_instr_str);
    
    printf("next 16 bytes in memory: \n");
    for (unsigned short i = program_counter.p; i < ((program_counter.p + 16) < 0xFFFF ? program_counter.p + 16 : 0xFFFF); i++) {
        printf("%x ", bus_read_data(i));
    }
    printf("\n");
}

char * last_instr() {
    return last_instr_str;
}

int * in_irq() {
    return &in_irq_val;
}

unsigned char * A() {
    return &AR;
}

unsigned char * X() {
    return &XR;
}

unsigned char * Y() {
    return &YR;
}

unsigned char * sp() {
    return &SPR;
}

unsigned char * N() {
    return &NS;
}

unsigned char * V() {
    return &VS;
}

unsigned char * B() {
    return &BS;
}

unsigned char * D() {
    return &DS;
}

unsigned char * I() {
    return &IS;
}

unsigned char * Z() {
    return &ZS;
}

unsigned char * C() {
    return &CS;
}

unsigned char status_register() {
    unsigned char sr = NS;
    sr = (sr << 1) + VS;
    sr = (sr << 1) + 1;
    sr = (sr << 1) + BS;
    sr = (sr << 1) + DS;
    sr = (sr << 1) + IS;
    sr = (sr << 1) + ZS;
    sr = (sr << 1) + CS;
    return sr;
}

void set_status_register(unsigned char val) {
    CS = val & 0b1;
    ZS = (val >> 1) & 0b1;
    IS = (val >> 2) & 0b1;
    DS = (val >> 3) & 0b1;
    BS = (val >> 4) & 0b1;
    VS = (val >> 6) & 0b1;
    NS = (val >> 7) & 0b1;
}

ADDR * pc() {
    return &program_counter;
}
