#ifndef MODES_H
#define MODES_H

void set_quit_mode(int mode);
int get_quit_mode();

void set_clock_speed(int clock_speed);
int get_clock_speed();

int get_irq_available();
void set_irq_available(int available);

#endif
