#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ht16k33.h"

static char display_buffer[8];  // Buffer for digit/character representation

void ht16k33_init(int file) {
    char cmd[1] = {0x21};  // Turn on oscillator command
    if (write(file, cmd, 1) != 1) {
        perror("Failed to initialize HT16K33: turning on oscillator");
        exit(1);
    }
    cmd[0] = 0xEF;  // Set brightness to maximum
    if (write(file, cmd, 1) != 1) {
        perror("Failed to initialize HT16K33: setting brightness");
        exit(1);
    }
    cmd[0] = 0x81;  // Turn on display without blinking
    if (write(file, cmd, 1) != 1) {
        perror("Failed to initialize HT16K33: turning on display");
        exit(1);
    }
    ht16k33_clear(file);
}

void ht16k33_clear(int file) {
    for (int i = 0; i < 8; i++) {
        display_buffer[i] = 0x00;
    }
    ht16k33_update_display(file);
}

void ht16k33_set_digit(int file, int digit, int value, int dot) {
    if (digit >= 0 && digit < 4) {
        // Buffer undex überspringe ":" zwischen second and third sgment 
        int buffer_index = (digit < 2) ? digit : digit + 1;
        display_buffer[buffer_index] = digit_segments[value] | (dot ? 0x80 : 0);
    }
}



void ht16k33_update_display(int file) {
    char cmd[17];
    cmd[0] = 0x00;  // Start address for display data
    for (int i = 0; i < 8; i++) {
        cmd[1 + i * 2] = display_buffer[i];  // Load each byte of the display buffer into command array
        cmd[2 + i * 2] = 0x00; 
    }
    if (write(file, cmd, 17) != 17) {
        perror("Failed to update HT16K33 display");
        exit(1);
    }
}