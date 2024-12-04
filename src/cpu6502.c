#include <cpu6502.h>
#include <stdio.h>
#include <unistd.h>
#include <bus.h>
#include <stdlib.h>
#include <string.h>
#include <modes.h>
//#define DEBUG
// accumulator, x and y registers/
unsigned char A, X, Y;

// stack pointer
unsigned char sp;

// program counter
typedef union {
    unsigned short p;
    unsigned char c[2];
    struct {
        unsigned char l;
        unsigned char h;
    } s;
} ADDR;
ADDR pc;

// Status register
unsigned char N, V, B, D, I, Z, C;

char last_instr[16] = {0}; 

char * get_last_instr() {
    return last_instr;
}

int in_irq = 0;

void abs_a(ADDR* addr, int extra) {
    unsigned char h_i = pc.s.h;
    clock();
    pc.p++;
    addr->c[0] = bus_read_data(pc.p);
    clock();
    pc.p++;
    addr->c[1] = bus_read_data(pc.p);
    clock();
    pc.p++;
    unsigned char h_f = pc.s.h;

    if (h_f != h_i || extra) {
        //printf("page different\n");
        clock();
    }
}

void abs_x(ADDR* addr, int extra) {
    unsigned char h_i = pc.s.h;
    clock();
    pc.p++;
    addr->c[0] = bus_read_data(pc.p);
    clock();
    pc.p++;
    addr->c[1] = bus_read_data(pc.p);
    clock();
    addr->p += (char)X;
    clock();
    pc.p++;
    unsigned char h_f = pc.s.h;

    if (h_f != h_i) {
        //printf("page different\n");
        clock();
    }
}

void abs_y(ADDR* addr, int extra) {
    unsigned char h_i = pc.s.h;
    clock();
    pc.p++;
    addr->c[0] = bus_read_data(pc.p);
    clock();
    pc.p++;
    addr->c[1] = bus_read_data(pc.p);
    clock();
    addr->p += (char)Y;
    clock();
    pc.p++;
    unsigned char h_f = pc.s.h;

    if (h_f != h_i) {
        //printf("page different\n");
        clock();
    }
}

void imm(unsigned char* val) {
    clock();
    pc.p++;
    *val = bus_read_data(pc.p);
    clock();
    pc.p++;
}

void CLC() {
    C = 0;
    pc.p++;
    clock();
    clock();
#ifdef DEBUG
   printf("CLC\n");
#endif
   strcpy(last_instr, "CLC");
}

void ORA_A() {
     ADDR addr;
     abs_a(&addr, 1);
     unsigned char val = bus_read_data(addr.p);
     A = A | val;
     N = (A >> 7) & 0b1;
     Z = A == 0;

#ifdef DEBUG
     printf("ORA %x %x\n", val, A);
#endif
     strcpy(last_instr, "ORA");
}

void JSR() {
     ADDR addr;
     abs_a(&addr, 1);
     
     bus_write_data(0x0100 + sp, pc.c[1]);
     sp--;
     bus_write_data(0x0100 + sp, (pc.c[0]));
     sp--;
#ifdef DEBUG
     printf("JSR %x\n", addr.p);
#endif
     pc.p = addr.p;
     clock();
     clock();
     strcpy(last_instr, "JSR");
}

void ANDI() {
    unsigned char val;
    imm(&val);
    A = A & val;
    Z = A == 0 ? 1 : 0;
    N = ((A >> 7) & 0b1) == 1 ? 1 : 0; 
#ifdef DEBUG
   printf("AND %x\n", A);
#endif
   strcpy(last_instr, "AND");
}

void BITA() {
    ADDR addr;
    abs_a(&addr, 1);
    unsigned char val = bus_read_data(addr.p);
   
    N = (val >> 7) & 0b1;
    Z = (A & val) == 0 ? 1 : 0;
    V = (val >> 6) & 0b1;

#ifdef DEBUG
   printf("BITA %x\n", addr.p);
#endif
   strcpy(last_instr, "BIT");

}

void ROLA() {
    ADDR addr;
    abs_a(&addr, 1);
    
    unsigned char value = bus_read_data(addr.p);
    unsigned char temp = (value >> 7) & 0b1;
    
    value = ((value << 1) & 0xFF) + C;
     C = temp;
     Z = value == 0 ? 1 : 0;
     N = (value >> 7) & 0b1;
     bus_write_data(addr.p, value);
     clock();
     clock();
#ifdef DEBUG
     printf("ROL $(%x)\n", addr.p);
#endif
     strcpy(last_instr, "ROL");
}

void SEC() {
    C = 1;
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
   printf("SEC\n");
#endif
   strcpy(last_instr, "SEC");
}

void RTI() {
    sp++;
    unsigned char sr = bus_read_data(0x0100 + sp);
    C = sr & 0b1;
    sr = sr >> 1;
    Z = sr & 0b1;
    sr = sr >> 1;
    I = sr & 0b1;
    sr = sr >> 1;
    D = sr & 0b1;
    sr = sr >> 1;
    sr = sr >> 1;
    sr = sr >> 1;
    V = sr & 0b1;
    sr = sr >> 1;
    N = sr & 0b1;
    
    sp++;
    pc.c[0] = bus_read_data(0x0100 + sp);

    sp++;
    pc.c[1] = bus_read_data(0x0100 + sp);
    
    for (int i = 0; i < 6; i++) {
        clock();
    }
#ifdef DEBUG
   printf("RTI\n");
#endif
   strcpy(last_instr, "RTI");
   in_irq = 0;
}

void PHA() {
    bus_write_data(0x100 + sp, A);
    sp--;
#ifdef DEBUG
    printf("PHA %x\n", A);
#endif
    clock();
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "PHA");
}

void RTS() {
    sp++;
    pc.c[0] = bus_read_data(0x0100 + sp);
    sp++;
    pc.c[1] = bus_read_data(0x0100 + sp);
    for (int i = 0; i < 6; i++) {
        clock();    
    }
#ifdef DEBUG
     printf("RTS\n");
#endif
     strcpy(last_instr, "RTS");
}
void JMP_A() {
    ADDR addr;
    abs_a(&addr, 0);
    pc.p = addr.p;
#ifdef DEBUG
     printf("JMP $%x\n", addr.p);
#endif
     strcpy(last_instr, "JMP");
}

void CLI() {
    I = 0;
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
     printf("CLI\n");
#endif
     strcpy(last_instr, "CLI");
}

void PLA() {
    sp++;
    A = bus_read_data(0x100 + sp);
    
#ifdef DEBUG
    printf("PLA %x\n", A);
#endif
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    
    clock();
    clock();
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "PLA");
}

void ADC_IMM() {
    unsigned char val;
    imm(&val);
    
    unsigned char of = 0;
    if (((A >> 7) & 0b1) == ((val >> 7) & 0b1)) {
        of = 1;
    }
    unsigned short  temp = (unsigned short)A;
    temp += val;
    temp += (C & 0b1);

#ifdef DEBUG
   printf("ADC 0x%x\n", temp);
#endif
    
    C = (temp >> 8) & 0b1;
    A = (unsigned char)(temp & 0xFF);
    N = (temp >> 7) & 0b1;
    Z = temp == 0 ? 1 : 0;
    V = (of == 1 && ((val >> 7) & 0b1) != N) ? 1 : 0;
    strcpy(last_instr, "ADC");
}

void RORA() {
    unsigned char carry = A & 0b1;
    A = A >> 1;
    A = (C << 7) + A;
    C = carry;
#ifdef DEBUG
    printf("ROR (A) %x\n", A);
#endif
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "ROR");
}

void SEI() {
    I = 1;
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
     printf("SEI\n");
#endif
     strcpy(last_instr, "SEI");
}

void TXA() {
    A = X;
    N = ((A >> 7) & 0b1) == 1 ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;

#ifdef DEBUG
   printf("TXA\n");
#endif
   strcpy(last_instr, "TXA");
}

void DEY() {
    Y--;
    N = ((Y >> 7) & 0b1) == 1 ? 1 : 0;
    Z = Y == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;

#ifdef DEBUG
    printf("DEY\n");
#endif
    strcpy(last_instr, "DEY");
}

void STY_A() {
    ADDR addr;
    abs_a(&addr, 1);
    bus_write_data(addr.p, Y);
    
#ifdef DEBUG
    printf("STY $%x\n", addr.p);
#endif
    strcpy(last_instr, "STY");
}

void STA_A() {
    ADDR addr;
    abs_a(&addr, 1);
    bus_write_data(addr.p, A);
    
#ifdef DEBUG
    printf("STA $%x\n", addr.p);
#endif
    strcpy(last_instr, "STA");
}


void BCC() {
    clock();
    clock();
    pc.p++;
    unsigned char data;
    data = bus_read_data(pc.p);
    pc.p++;
    
    //printf("Z: %d\n", Z);
    if (C == 0) {
        ADDR pc_orig;
        pc_orig.p = pc.p;
        pc.p = (unsigned short)((short)pc.p + (char) data);
        clock();
        if (pc_orig.s.h != pc.s.h) {
            clock();
        }
    }

#ifdef DEBUG
    printf("BCC %d: %d\n", (char)data, Z);
#endif
    strcpy(last_instr, "BCC");
}

void TYA() {
    A = Y;
    N = ((A >> 7) & 0b1) == 1 ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;

#ifdef DEBUG
   printf("TYA\n");
#endif
   strcpy(last_instr, "TYA");
}

void STA_AY() {
    ADDR addr;
    abs_y(&addr, 1);
    bus_write_data(addr.p, A);
    
#ifdef DEBUG
    printf("STA,Y $%x\n", addr.p);
#endif
    strcpy(last_instr, "STA,Y");
}
void TXS() {
    sp = X;
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
   printf("TXS\n");
#endif
   strcpy(last_instr, "TXS");
}

void LDY_IMM() {
    unsigned char val;
    imm(&val);
    Y = val;
    N = (((Y >> 7) & 0b1) == 1) ? 1 : 0;
    Z = X == 0 ? 1 : 0;
#ifdef DEBUG
   printf("LDY #(%x)\n", val);
#endif
   strcpy(last_instr, "LDY");
}

void LDX_IMM() {
    unsigned char val;
    imm(&val);
    X = val;
    N = (((X >> 7) & 0b1) == 1) ? 1 : 0;
    Z = X == 0 ? 1 : 0;
#ifdef DEBUG
   printf("LDX #(%x)\n", val);
#endif
   strcpy(last_instr, "LDX");
}

void LDA_IMM() {
    unsigned char val;
    imm(&val);
    A = val;
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
#ifdef DEBUG
   printf("LDA #(%x)\n", val);
#endif
   strcpy(last_instr, "LDA");
}

void LDA_AY() {
    ADDR addr;
    abs_y(&addr, 0);
    A = bus_read_data(addr.p);    
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
#ifdef DEBUG
    printf("LDA,Y $(%x),%x\n", addr.p, X);
#endif
    strcpy(last_instr, "LDA,Y");
}
void LDA_AX() {
    ADDR addr;
    abs_x(&addr, 0);
    A = bus_read_data(addr.p);    
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
#ifdef DEBUG
    printf("LDA,X $(%x),%x\n", addr.p, X);
#endif
    strcpy(last_instr, "LDA,X");
}

void INY() {
    Y++;
    N = ((Y >> 7) & 0b1) == 1 ? 1 : 0;
    Z = Y == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "INY");
}

void DEX() {
    X--;
    N = ((X >> 7) & 0b1) == 1 ? 1 : 0;
    Z = X == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "DEX");
}

void LDA_A() {
    ADDR addr;
    abs_a(&addr, 1);
    A = bus_read_data(addr.p);    
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
#ifdef DEBUG
   printf("LDA $(%x)\n", addr.p);
#endif
   strcpy(last_instr, "LDA");
}

void TAX() {
    X = A;
    N = ((X >> 7) & 0b1) == 1 ? 1 : 0;
    Z = X == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;

#ifdef DEBUG
   printf("TAY\n");
#endif
   strcpy(last_instr, "TAX");
}

void TAY() {
    Y = A;
    N = ((Y >> 7) & 0b1) == 1 ? 1 : 0;
    Z = Y == 0 ? 1 : 0;
    clock();
    clock();
    pc.p++;

#ifdef DEBUG
   printf("TAY\n");
#endif
   strcpy(last_instr, "TAY");
}

void BNE() {
    clock();
    clock();
    pc.p++;
    unsigned char data;
    data = bus_read_data(pc.p);
    pc.p++;
    
    //printf("Z: %d\n", Z);
    if (Z == 0) {
        ADDR pc_orig;
        pc_orig.p = pc.p;
        pc.p = (unsigned short)((short)pc.p + (char) data);
        clock();
        if (pc_orig.s.h != pc.s.h) {
            clock();
        }
    }

#ifdef DEBUG
    printf("BNE %d: %d\n", (char)data, Z);
#endif
    strcpy(last_instr, "BNE");
}

void PHX() {
    bus_write_data(0x100 + sp, X);
    sp--;
#ifdef DEBUG
    printf("PHX %x\n", X);
#endif
    clock();
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "PHX");
}

void INX() {
    X++;
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
    printf("INX\n");
#endif
    strcpy(last_instr, "INX");
}

void SBC_IMM() {
    unsigned char val;
    imm(&val);
    
    unsigned char of = 0;
    if (((A >> 7) & 0b1) == ((val >> 7) & 0b1)) {
        of = 1;
    }
    short temp = (short)A;
    temp += (C << 8);
    temp -= val;
    

#ifdef DEBUG
   printf("SBC 0x%x\n", temp);
#endif
    
    C = (temp >> 8) & 0b1;
    A = (unsigned char)(temp & 0xFF);
    N = (temp >> 7) & 0b1;
    Z = temp == 0 ? 1 : 0;
    V = (of == 1 && ((val >> 7) & 0b1) != N) ? 1 : 0;
    strcpy(last_instr, "SBC");
}
    
void NOP() {
    clock();
    clock();
    pc.p++;
#ifdef DEBUG
   printf("NOP\n");
#endif
   strcpy(last_instr, "NOP");
}

void INC_A() {
    ADDR addr;
    abs_a(&addr, 1);
    unsigned char val = bus_read_data(addr.p);
    val++;
    N = (val >> 7) & 0b1;
    Z = val == 0 ? 1 : 0;
    bus_write_data(addr.p, val);
    clock();
    clock();

#ifdef DEBUG
    printf("INC %x\n", val);
#endif
   strcpy(last_instr, "INC");

}
void BEQ() {
    clock();
    clock();
    pc.p++;
    unsigned char data;
    data = bus_read_data(pc.p);
    pc.p++;

    if (Z == 1) {
        ADDR pc_orig;
        pc_orig.p = pc.p;
        pc.p = (unsigned short)(short)pc.p + ((char) data);
        clock();
        if (pc_orig.s.h != pc.s.h) {
            clock();
        }
    }

#ifdef DEBUG
    printf("BEQ %x: %d\n", data, Z);
#endif
    strcpy(last_instr, "BEQ");
}


void PLX() {
    sp++;
    X = bus_read_data(0x100 + sp);
    
#ifdef DEBUG
    printf("PLX %x\n", A);
#endif
    N = (((A >> 7) & 0b1) == 1) ? 1 : 0;
    Z = A == 0 ? 1 : 0;
    
    clock();
    clock();
    clock();
    clock();
    pc.p++;
    strcpy(last_instr, "PLX");
}

void start_interrupt() {
    bus_write_data(0x0100 + sp, pc.c[1]);
    sp--;
    bus_write_data(0x0100 + sp, (pc.c[0]));
    sp--;
    
    unsigned char sr = N;
    sr = (sr << 1) + V;
    sr = (sr << 1) + 1;
    sr = (sr << 1) + B;
    sr = (sr << 1) + D;
    sr = (sr << 1) + I;
    sr = (sr << 1) + Z;
    sr = (sr << 1) + C;
    bus_write_data(0x100 + sp, sr);
    sp--;

    for (int i = 0; i < 7; i++) {
        clock();
    }
}

void interrupt() {
    if (I != 0) {
        return;
    }
    set_irq_available(0);
    in_irq = 1;
    start_interrupt();
    pc.c[0] = bus_read_data(0xFFFE);
    pc.c[1] = bus_read_data(0xFFFF);
}


void non_maskable_interrupt() {
    start_interrupt();
    pc.c[0] = bus_read_data(0xFFFA);
    pc.c[1] = bus_read_data(0xFFFB);
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
    N = 0;
    V = 0;
    B = 0;
    D = 0;
    I = 1;
    Z = 0;
    C = 0;

    // set registers to zero.
    A = 0;
    X = 0;
    Y = 0;
    
    // set stack pointer
    sp = 0xFD;
    // initialize data pointer.
    pc.c[0] = bus_read_data(0xFFFC);
    clock();
    pc.c[1] = bus_read_data(0xFFFD);
    clock();
    strcpy(last_instr, "RESET");
}

void traceback() {
    unsigned char instr = bus_read_data(pc.p);
    printf("DEBUG DATA: A: %x:%d, X: %X:%d, Y: %X:%d, C: %d, N: %d, Z: %d, V: %d\n", A, A, X,  X, Y, Y, (int)C, (int)N, (int)Z, (int)V);
    printf("SP: %x\n\n", sp);
    printf("instruction: %x at %x. Exiting.\n", instr, pc.p);
    
    printf("next 16 bytes in memory: \n");
    for (unsigned short i = pc.p; i < ((pc.p + 16) < 0xFFFF ? pc.p + 16 : 0xFFFF); i++) {
        printf("%x ", bus_read_data(i));
    }
    printf("\n");
}

void run_instr() {
    if (get_irq_available() == 0 && in_irq == 0) {
        interrupt();
    }

    unsigned char instr = bus_read_data(pc.p);
    switch (instr) {
    case 0x0D:
        ORA_A();
        break;
    case 0x18:
        CLC();
        break;
    case 0x20:
        JSR();
        break;
    case 0x29:
        ANDI();
        break;
    case 0x2C:
        BITA();
        break;
    case 0x2E:
        ROLA();
        break;
    case 0x38:
        SEC();
        break;
    case 0x40:
        RTI();
        break;
    case 0x48:
        PHA();
        break;
    case 0x4C:
        JMP_A();
        break;
    case 0x58:
        CLI();
        break;
    case 0x60:
        RTS();
        break;
    case 0x68:
        PLA();
        break;
    case 0x69:
        ADC_IMM();
        break;
    case 0x6A:
        RORA();
        break;
    case 0x78:
        SEI();
        break;
    case 0x88:
        DEY();
        break;
    case 0x8A:
        TXA();
        break;
    case 0x8C:
        STY_A();
        break;
    case 0x8D:
        STA_A();
        break;
    case 0x90:
        BCC();
        break;
    case 0x98:
        TYA();
        break;
    case 0x99:
        STA_AY();
        break;
    case 0x9A:
        TXS();
        break;
    case 0xA0:
        LDY_IMM();
        break;
    case 0xA2:
        LDX_IMM();
        break;
    case 0xAD:
        LDA_A();
        break;
    case 0xAA:
        TAX();
        break;
    case 0xA8:
        TAY();
        break;
    case 0xA9:
        LDA_IMM();
        break;
    case 0xB9:
        LDA_AY();
        break;
    case 0xBD:
        LDA_AX();
        break;
    case 0xC8:
        INY();
        break;
    case 0xCA:
        DEX();
        break;
    case 0xD0:
        BNE();
        break;
    case 0xDA:
        PHX();
        break;
    case 0xE8:
        INX();
        break;
    case 0xE9:
        SBC_IMM();
        break;
    case 0xEA:
        NOP();
        break;
    case 0xEE:
        INC_A();
        break;
    case 0xF0:
        BEQ();
        break;
    case 0xFA:
        PLX();
        break;

    default:
        printf("Unknown instruction: %x at %x. Exiting.\n", instr, pc.p);
        strcpy(last_instr, "ERROR");
#ifndef NCURSES
        traceback();
        exit(1);
#endif
        break;
    }
}
