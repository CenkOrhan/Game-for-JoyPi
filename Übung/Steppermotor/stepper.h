#ifndef STEPPER_H
#define STEPPER_H

// Setup function for GPIO
void setup();

// Function to turn the motor by a specified number of steps
void turnSteps(int steps, int interval, char direction);

#endif
