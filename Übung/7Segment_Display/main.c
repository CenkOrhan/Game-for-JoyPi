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
    for () {
            //AUFGABE HIER IMPLEMNTIEREN
    }

    close(file);
    return 0;
}
