#ifndef MAX7219_H
#define MAX7219_H

// Registeradressen des MAX7219
#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// Funktionen
void max7219_init(int file);
void max7219_all_on(int file);
void max7219_all_off(int file);
void max7219_blink(int file);
void max7219_set_pattern(int file, unsigned char pattern[8]);
void setup_spi(int *file);
unsigned char* get_pattern(int num);

#endif // MAX7219_H
