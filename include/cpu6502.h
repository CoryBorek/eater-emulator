#ifndef CPU6502_H
#define CPU6502_H
#include <stdbool.h>
#include <cpu/addressing.h>
/**
 * Triggers a regular interrupt.
 *
 */
void interrupt();

/**
 * Triggers a non-maskable interrupt
 */
void non_maskable_interrupt();

/**
 * Runs the instruction at the current program counter.
 */
void run_instr();

/**
 * Triggers an unknown instruction, and throws an error.
 */
void unknown();

/**
 * clocks the CPU.
 */
void clock();

/**
 * Resets the CPU, and triggers said reset.
 */
void reset();

/**
 * Logs a traceback and status of the CPU, to help debug errors.
 */
void traceback();

/**
 * Returns the string value of the last instruction.
 */
char * last_instr();

/**
 * Returns the in_irq variable.
 */
int * in_irq();

/**
 * A Register
 */
unsigned char * A();

/**
 * X Register
 */
unsigned char * X();

/**
 * Y Register
 */
unsigned char * Y();

/**
 * Returns the Stack pointer
 */
unsigned char * sp();

/**
 * Negative flag
 */
unsigned char * N();

/**
 * Overflow flag
 */
unsigned char * V();

/**
 * Break flag
 */
unsigned char * B();

/**
 * Decimal Flag
 */
unsigned char * D();

/**
 * Interrupt Disable
 */
unsigned char * I();

/**
 * Zero flag
 */
unsigned char * Z();

/**
 * Carry flag
 */
unsigned char * C();

/**
 * Returns the full status register
 */
unsigned char status_register();

/**
 * Sets the status register
 */
void set_status_register(unsigned char val);

/**
 * returns the program counter
 */
ADDR * pc();

#endif
