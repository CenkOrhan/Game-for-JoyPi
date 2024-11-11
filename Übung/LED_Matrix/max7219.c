#include "max7219.h"
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

static void write_register(int file, unsigned char reg, unsigned char data) {
    unsigned char buf[2] = {reg, data};
    write(file, buf, sizeof(buf));
}

void setup_spi(int *file) {
    *file = open("/dev/spidev0.1", O_RDWR);
    if (*file < 0) {
        perror("Failed to open the SPI device");
        return;
    }

    unsigned char mode = SPI_MODE_0;
    if (ioctl(*file, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        close(*file);
        *file = -1;
        return;
    }

    unsigned int speed = 10000000;
    if (ioctl(*file, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("Failed to set SPI speed");
        close(*file);
        *file = -1;
        return;
    }
}

void max7219_init(int file) {
    write_register(file, MAX7219_REG_SCANLIMIT, 0x07);
    write_register(file, MAX7219_REG_DECODEMODE, 0x00);
    write_register(file, MAX7219_REG_DISPLAYTEST, 0x00);
    write_register(file, MAX7219_REG_INTENSITY, 0x08);
    write_register(file, MAX7219_REG_SHUTDOWN, 0x01);
}

void max7219_all_on(int file) {
    for (int i = 0; i < 8; i++) {
        write_register(file, MAX7219_REG_DIGIT0 + i, 0xFF);
    }
}

void max7219_all_off(int file) {
    for (int i = 0; i < 8; i++) {
        write_register(file, MAX7219_REG_DIGIT0 + i, 0x00);
    }
}

void max7219_blink(int file) {
    for (int i = 0; i < 3; i++) {
        max7219_all_on(file);
        usleep(200000); // 200 Millisekunden an
        max7219_all_off(file);
        usleep(200000); // 200 Millisekunden aus
    }
}

void max7219_set_pattern(int file, unsigned char pattern[8]) {
    for (int i = 0; i < 8; i++) {
        write_register(file, MAX7219_REG_DIGIT0 + i, pattern[i]);
    }
}


unsigned char* get_pattern(int num) {
    static unsigned char patterns[9][8] = {
        {0xFF, 0x81, 0xBD, 0xA5, 0xA5, 0xBD, 0x81, 0xFF}, // Lächelndes Gesicht
        {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF}, // Horizontale Linien
        {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81}, // X-Muster
        {0x00, 0x66, 0x89, 0x8F, 0x8F, 0x89, 0x66, 0x00}, // Herz
        {0x00, 0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0x7E, 0x00}, // Smile
        {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55}, // Schachbrett
        {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00}, // Vertikale Linien
        {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF}, // Rad
        {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}  // Diagonale Linie
    };
    return patterns[num - 1];
}
