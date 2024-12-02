#include <stdio.h>
#include <bus.h>
#include <cpu6502.h>

int main(int argc, char * argv[]) {
    init_bus();
    reset();

    // run 100 instructions;

    for (int i = 0; i < 5; i++) {
        run_instr();
    }

    printf("done!\n");
}
