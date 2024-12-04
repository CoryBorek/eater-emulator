#include <acia65c51.h>
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <stdlib.h>
#include <sys/ioctl.h>
unsigned char transmit_data = 0;
unsigned char read_data = 0;

unsigned char status_register = 0;
unsigned char control_register = 0;
unsigned char command_register = 0;
// control register bits
unsigned char stop_bit = 0;
unsigned char word_length = 0;

unsigned char receiver_clock_source = 0;

unsigned char selected_baud_rate = 0;

// command register bits
unsigned char parity_mode_control = 0;
unsigned char parity_mode_enabled = 0;
unsigned char receiver_echo_mode = 0;
unsigned char transmitter_interrupt_control = 0;
unsigned char receiver_interrupt_request_disabled = 0;
unsigned char data_terminal_ready = 0;


int serial_port = 0;

  // Create new termios struct, we call it 'tty' for convention
struct termios tty;



char * file;
void init_acia(char* file_path) {
    file = file_path;
    if (strcmp(file_path, "") == 0) {
        return;
    }
    serial_port = open(file_path, O_RDWR);

      // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        exit(1);
    }
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    //tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag |= ONLCR;
    tty.c_oflag &= ~OCRNL;
    tty.c_lflag &= ~ICANON;
    tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

}

void deinit_acia() {
    if (strcmp(file, "") == 0) {
        return;
    }
    close(serial_port);
}

void acia_write(unsigned char reg, unsigned char data) {
    
    reg &= 0b11;
    switch (reg) {
    case 0:
        transmit_data = data;
        unsigned char msg[] = {data};
        write(serial_port, msg, sizeof(msg));
        break;
    case 1:
        // programmed reset (data is don't care)
        break;
    case 2:
        command_register = data;
        
        parity_mode_control = (data >> 6) & 0b11;
        
// 00 odd parity
// 01 even parity
// 10 mark parity bit transmitted,parity check disabled
// 11 space parity bit transmitted parity check disabled
        
        parity_mode_enabled = (data >> 5) & 0b1;

        if (parity_mode_enabled == 1) {
            tty.c_cflag |= PARENB; // parity enabled
            switch (parity_mode_control) {
            case 0b00:
                tty.c_cflag |= PARODD;
                tty.c_cflag |= INPCK;
                break;
            case 0b01:
                tty.c_cflag &= ~PARODD;
                tty.c_cflag |= INPCK;
                break;
            case 0b10:
            case 0b11:
                tty.c_cflag &= ~INPCK;
                break;
            }
        } else {
            tty.c_cflag &= ~PARENB; // parity disabled
            tty.c_cflag &= ~INPCK; // parity check disabled
            
        }
        receiver_echo_mode = (data >> 4) & 0b1;
        transmitter_interrupt_control = (data >> 2) & 0b11;

        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHONL; // Disable new-line echo

        if (receiver_echo_mode == 1 && transmitter_interrupt_control == 0) {
            tty.c_lflag |= ECHO;
            tty.c_lflag |= ECHONL; // Disable new-line echo
        }
        
        receiver_interrupt_request_disabled = (data >> 1) & 0b1;
        data_terminal_ready = data & 0b1;
        
        break;
    case 3:
        control_register = data;

        stop_bit = (data >> 7) & 0b1;
        word_length = (data >> 5) & 0b11;
        if (stop_bit == 0) {
            tty.c_cflag &= ~CSTOPB;
        } else {
            if (word_length == 0b00 && parity_mode_enabled == 1) {
                tty.c_cflag &= ~CSTOPB;
            } else if (word_length == 0b11 && parity_mode_enabled == 0) {
                tty.c_cflag &= ~CSTOPB;
            } else {
                tty.c_cflag |= CSTOPB;
            }
        }

        switch (word_length) {
        case 0b00:
            tty.c_cflag |= CS8;
            tty.c_cflag &= ~CS7;
            tty.c_cflag &= ~CS6;
            tty.c_cflag &= ~CS5;
            break;
        case 0b01:
            tty.c_cflag |= CS7;
            tty.c_cflag &= ~CS8;
            tty.c_cflag &= ~CS6;
            tty.c_cflag &= ~CS5;
            break;
        case 0b10:
            tty.c_cflag |= CS6;
            tty.c_cflag &= ~CS7;
            tty.c_cflag &= ~CS8;
            tty.c_cflag &= ~CS5;
            break;
        case 0b11:
            tty.c_cflag |= CS5;
            tty.c_cflag &= ~CS7;
            tty.c_cflag &= ~CS6;
            tty.c_cflag &= ~CS8;

            break;
        }
        
        
        receiver_clock_source = (data >> 4) & 0b1;
        selected_baud_rate = data & 0b1111;
        if (receiver_clock_source == 1) {
            int baud_rate = B0;
            switch (selected_baud_rate) {
            case 0:
                baud_rate = B0;
                break;
            case 1:
                baud_rate = B50;
                break;
            case 2:
                baud_rate = B75;
                break;
            case 3:
                baud_rate = B110;
                break;
            case 4:
                baud_rate = B134;
                break;
            case 5:
                baud_rate = B150;
                break;
            case 6:
                baud_rate = B300;
                break;
            case 7:
                baud_rate = B600;
                break;
            case 8:
                baud_rate = B1200;
                break;
            case 9:
                baud_rate = B1800;
                break;
            case 10:
                baud_rate = B2400;
                break;
            case 12:
                baud_rate = B4800;
                break;
            case 14:
                baud_rate = B9600;
                break;
            case 15:
                baud_rate = B19200;
                break;
            }
            
            cfsetispeed(&tty, baud_rate);
            cfsetospeed(&tty, baud_rate);

        }

        break;
    }
}

unsigned char acia_read(unsigned char reg) {
    reg &= 0b11;
    switch (reg) {
    case 0:
        status_register &= ~0b00001000;
        char read_buf[1];
        
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
        
        if (num_bytes < 0) {
            printf("num bytes is invalid...\n");
            exit(-1);
        }
        if (num_bytes > 0) {
            read_data = read_buf[0];
        }
        
        return read_data;
        break;
    case 1:
        status_register |= 0b00010000;
        int bytes;
        ioctl(serial_port, FIONREAD, &bytes);
        if (bytes > 0) {
            status_register |= 0b00001000;
        } else {
            status_register &= ~0b00001000;
        }
        return status_register;
        break;
    case 2:
        return command_register;
        break;
    case 3:
        return control_register;
        break;
    }
    return 0;
}
