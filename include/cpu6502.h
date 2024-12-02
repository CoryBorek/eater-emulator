#ifndef CPU6502_H
#define CPU6502_H
#include <stdbool.h>

void interrupt();

void non_maskable_interrupt();

void run_instr();

void clock();

void reset();

#endif
