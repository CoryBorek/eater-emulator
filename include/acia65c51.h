#ifndef ACIA65C51_H
#define ACIA65C51_H

void acia_write(unsigned char register, unsigned char data);

unsigned char acia_read(unsigned char register);

void init_acia(char* file_path);
void deinit_acia();
#endif
