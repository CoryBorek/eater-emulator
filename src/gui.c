#include <stdio.h>
#include <bus.h>
#include <eeprom26c256.h>
#include <via65c22.h>
#include <cpu6502.h>
#include <HD44780U.h>
#include <ram.h>
#include <ncurses.h>
#include <string.h>
#include <modes.h>
#include <acia65c51.h>

void display_center(int yy, char * str)
{
  // Get the screen size
  int y, x;
  getmaxyx(stdscr, y, x);
  
  
  y = y / 2 + yy;
  // Compute starting location for string (centre)
  x = (x - strlen(str))/2;

  // Write the string to the window
  mvwprintw(stdscr, y, x, str);

  // Make sure the screen is updated
  refresh();
}

void char_to_binary_string(char* str, unsigned char n) {
    
    unsigned char i;

    int iter = 0;
    for (i = 1 << 7; i > 0; i = i / 2) {
        if ((n & i) != 0) {
            str[iter] = '1';
        }
        else {
            str[iter] = '0';
        }
        iter++;
    }
    str[iter] = 0;
}

int main(int argc, char * argv[]) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    // Get the screen size
    int s_y, s_x;
    getmaxyx(stdscr, s_y, s_x);
    
    // Write a message at the bottom left of the screen
    mvwprintw(stdscr, s_y-1, s_x-s_x, "Press q to close");
    
    display_center(-8, "6502 BE Emulator");
    
    init_bus();
    init_ram(0x00, 0x4000);
    set_clock_speed(1);
    char * acia_file = "";
    if (argc > 1) {
        FILE * fptr;
        fptr = fopen(argv[1], "r");
        unsigned char data[32768];
        
        fread(data, sizeof(data), 1, fptr);
        eeprom_init(data, 0x8000, 0x8000);
        fclose(fptr);
        if (argc > 2) {
            acia_file = argv[2];
        }
    }
    init_acia(acia_file);

    reset();

    wtimeout(stdscr, 0);
    int run = 1;
    int err = 0;
    int last_char = getch();
    char prev_dram[80];
    unsigned char prev_b = 0;
    unsigned char prev_a = 0;

    display_center(-1, "VIA REGISTER B:");
    display_center(1, "VIA REGISTER A:");

    while(run) {
        run_instr();

        
        char str[17];
        char str2[17];
        
        char * dram = get_DRAM();
        if (strcmp(prev_dram, dram) != 0) {
            display_center(-5, "                ");
            display_center(-4, "                ");
        }
        strcpy(prev_dram, dram);
        for (int i = 0; i < 16; i++) {
            str[i] = dram[i];
            str2[i] = dram[41+i];
        }

        str[16] = 0;
        str2[16] = 0;
        display_center(-5, str);
        display_center(-4, str2);
        
        char instr_str[17];
        strcpy(instr_str, last_instr());
        for (int i = strlen(instr_str); i < 17; i++) {
            instr_str[i] = ' ';
        }
        instr_str[16] = 0;
        display_center(4, instr_str);
        
        if (strcmp(last_instr(), "ERROR") == 0) {
            err = 1;
            run = 0;
            break;
        }
        
        
        
        unsigned char curr_a = out_a();
        unsigned char curr_b = out_b();
        
        if (prev_a != curr_a) {
            char via_a[32];
            display_center(2, "                ");
            char_to_binary_string(via_a, curr_a);
            display_center(2, via_a);
        }

        if (prev_b != curr_b) {
            char via_b[32];
            char_to_binary_string(via_b, curr_b);
            display_center(0, "                ");
            display_center(0, via_b);
        }

        prev_a = curr_a;
        prev_b = curr_b;

        switch (last_char) {
        case ERR:
            break;
        case 'q':
            if (get_quit_mode() == 0) run = 0;
            break;
        case 'c':
            set_irq_available(1);
            wtimeout(stdscr, -1);
            display_center(5, "Input a new timeout.");
            char clock_str[10] = {0};
            char current = 0;
            int iter = 0;
            while (iter < 10 && (current = getch()) != '\n') {
                
                while (current < '0' || current > '9') {
                    current = getch();
                }
                clock_str[iter] = (char)current;
                iter++;
                clock_str[iter] = 0;
                display_center(6, "                    ");
                display_center(6, clock_str);
            }
            int speed = 0;
            for (int i = 0; i < strlen(clock_str); i++) {
                speed *= 10;
                speed += (clock_str[i] - '0');
            }
            speed = speed == 0 ? 1 : speed;
            set_clock_speed(speed);
            display_center(5, "                    ");
            display_center(6, "                    ");
            wtimeout(stdscr, 0);
            break;
        case 'n': 
            set_irq_available(1);
            non_maskable_interrupt();
            break;
        case 'i':
            ca1(0);
            break;
        case 'r':
            reset();
            break;
        default:
            set_irq_available(1);
            if (get_quit_mode() == 1) run = 0;    
            break;
        }
        last_char = wgetch(stdscr);
    }
    
    endwin();
    
    if (err) {
        traceback();
    }

    eeprom_deinit();
    printf("done!\n");
}
