#ifndef HT16K33_H
#define HT16K33_H

// I2C-Adresse des HT16K33
#define HT16K33_ADDRESS 0x70

// Bitmasken für Ziffern 0-9 auf einer 7-Segment-Anzeige
static const char digit_segments[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void ht16k33_init(int file);
void ht16k33_clear(int file);
void ht16k33_set_digit(int file, int digit, int value, int dot);
void ht16k33_update_display(int file);

#endif // HT16K33_H
