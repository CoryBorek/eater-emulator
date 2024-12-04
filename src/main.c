#include <stdio.h>
#include <bus.h>
#include <eeprom26c256.h>
#include <via65c22.h>
#include <cpu6502.h>
#include <HD44780U.h>
#include <ram.h>
#include <modes.h>

void bin(unsigned char  n) {
    unsigned char i;
    printf("0");
    for (i = 1 << 7; i > 0; i = i / 2) {
        if ((n & i) != 0) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
}

int main(int argc, char * argv[]) {
    init_bus();
    init_ram(0x00, 0x4000);
    set_clock_speed(1);
    if (argc > 1) {
        FILE * fptr;
        fptr = fopen(argv[1], "r");
        unsigned char data[32768];
        
        fread(data, sizeof(data), 1, fptr);
        eeprom_init(data, 0x8000, 0x8000);
        fclose(fptr);
    }
    reset();

    // run 100 instructions;
    int run = 1;
    while(run) {
        run++;
        if (run == 100) {
            ca1(0);
        }
        run_instr();
        print_display();
        //unsigned char via_b = out_b();
        //printf("VIA REGISTER B: ");
        //bin(via_b);
        //printf(" %c\n", (char)via_b);
        //display_read_instruction();
    }

    eeprom_deinit();

    printf("done!\n");
}
