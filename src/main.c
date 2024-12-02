#include <stdio.h>
#include <bus.h>
#include <eeprom26c256.h>
#include <via65c22.h>
#include <cpu6502.h>

int main(int argc, char * argv[]) {
    init_bus();

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

    for (int i = 0; i < 15; i++) {
        run_instr();
        unsigned char via_b = out_b();
        printf("VIA REGISTER B: 0x%02x\n", (int)via_b);
    }

    eeprom_deinit();

    printf("done!\n");
}
