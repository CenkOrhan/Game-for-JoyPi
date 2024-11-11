#include <stdio.h>
#include "stepper.h"

int main() {
    int steps, interval;
    char direction;
    int lineCount = 1;  // To keep track of the line numbers for better error messages
    
    FILE *file = fopen("commands.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    setup();
    

    while (fscanf(file, "%d %d %c\n", &steps, &interval, &direction) == 3) {
        if (steps < 20 || steps > 500) {
            printf("Error on line %d: Step count %d is out of range (20-500).\n", lineCount, steps);
        } else if (interval < 3 || interval > 20) {
            printf("Error on line %d: Interval %d is out of range (3-20).\n", lineCount, interval);
        } else if (direction != 'L' && direction != 'R') {
            printf("Error on line %d: Direction '%c' is invalid. Use 'L' or 'R'.\n", lineCount, direction);
        } else {
            // Print current command values
            printf("Processing command on line %d: steps: %d , interval: %d , direction %c\n", lineCount, steps, interval, direction);
            turnSteps(steps, interval, direction);
        }
        lineCount++;
    }

    fclose(file);
    return 0;
}
