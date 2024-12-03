#include <via65c22.h>
#include <stdio.h>
#include <stdlib.h>
//unsigned char ID = 0;
//unsigned char S = 0;
//unsigned char D = 0;
//unsigned char C = 0;
//unsigned char B = 0;

//unsigned char DL = 0;
//unsigned char N = 0;
//unsigned char F = 0;

char DRAM[80] = {0};

char display_line1[18] = {0};
char display_line2[18] = {0};

int cursor = 0;
void clear_display() {
    for (int i = 0; i < 16; i++) {
        display_line1[i] = ' ';
        display_line2[i] = ' ';
    }
    display_line1[16] = '\n';
    display_line2[16] = '\n';
    
    display_line1[17] = 0;
    display_line2[17] = 0;
    //printf("display cleared!");
}

void return_home() {
    cursor = 0;
}

void cursor_display_shift(unsigned char SC, unsigned char RL) {

}

void print_display() {
    printf("%s", display_line1);
    printf("%s", display_line2);
}

void display_read_instruction() {
    unsigned char a_data = out_a();
    unsigned char b_data = out_b();

    unsigned char E = (a_data >> 7) & 0b1;
    unsigned char RW = (a_data >> 6) & 0b1;
    unsigned char RS = (a_data >> 5) & 0b1;
    
    if (E == 1) {
        if (RS == 0 && RW == 0) {
            if (b_data == 1) {
                clear_display();
            } else if (b_data <= 3) {
                return_home();
            } else if (b_data < 8) {
                //ID = (b_data >> 1) & 0b1;
                //S = b_data & 0b1;
            } else if (b_data < 16) {
                //D = (b_data >> 2) & 0b1;
                //C = (b_data >> 1) & 0b1;
                //B = b_data & 0b1;
            } else if (b_data < 32) {
                cursor_display_shift((b_data >> 3) & 0b1, (b_data >> 2) & 0b1);
            } else if (b_data > 64) {
                // DL = (b_data >> 4) & 0b1;
                //N = (b_data >> 3) & 0b1;
                //F = (b_data >> 2) & 0b1;
            }
        } else if (RS == 1 && RW == 0) {
            if (cursor < 16) display_line1[cursor] = (char) b_data;
            cursor++;
        } else if (RW == 1 && RS == 0) {
            in_b(0b00000000);
        }
    }
}
    
