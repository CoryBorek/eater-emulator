#include <modes.h>


int quit_mode = 0;
int clock_speed = 1;
int irq_available = 1;
void set_quit_mode(int mode) {
    quit_mode = mode;
}

int get_quit_mode() {
    return quit_mode;
}

void set_clock_speed(int speed) {
    clock_speed = speed;
}

int get_clock_speed() {
    return clock_speed;
}


int get_irq_available() {
    return irq_available;
}

void set_irq_available(int available) {
    irq_available = available;
}
