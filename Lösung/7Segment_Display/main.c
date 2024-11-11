#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ht16k33.h"


int main() {
    int file;
    char *filename = "/dev/i2c-1";

    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        return 1;
    }

    if (ioctl(file, I2C_SLAVE, HT16K33_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave.");
        close(file);
        return 1;
    }

    ht16k33_init(file);

    // Countdown from 1000 to 0
    for (int i = 1000; i >= 0; i--) {
        ht16k33_clear(file);
        ht16k33_set_digit(file, 0, (i / 1000) % 10, 0);  // Thousands
        ht16k33_set_digit(file, 1, (i / 100) % 10, 0);   // Hundreds
        ht16k33_set_digit(file, 2, (i / 10) % 10, 0);    // Tens
        ht16k33_set_digit(file, 3, i % 10, 0);           // Units
        ht16k33_update_display(file);
        usleep(500000);  // Update every 500 milliseconds
    }

    close(file);
    return 0;
}
