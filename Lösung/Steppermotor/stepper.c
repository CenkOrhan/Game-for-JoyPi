#include <wiringPi.h>
#include "stepper.h"

// Define GPIO pin numbers
#define PIN_A 21
#define PIN_B 22
#define PIN_C 23
#define PIN_D 24

// Setup GPIO for stepper motor
void setup() {
    wiringPiSetup();
    pinMode(PIN_A, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_C, OUTPUT);
    pinMode(PIN_D, OUTPUT);
    digitalWrite(PIN_A, LOW);
    digitalWrite(PIN_B, LOW);
    digitalWrite(PIN_C, LOW);
    digitalWrite(PIN_D, LOW);
}

// Function to perform a single step
static void performStep(int seq) {
    digitalWrite(PIN_A, seq & 0x1);
    digitalWrite(PIN_B, (seq >> 1) & 0x1);
    digitalWrite(PIN_C, (seq >> 2) & 0x1);
    digitalWrite(PIN_D, (seq >> 3) & 0x1);
}

// Full step sequence
void fullStep(int stepIndex, char direction) {
    int seq;
    if (direction == 'R') {
        seq = 0b1000 >> (stepIndex % 4); // Rotate right
    } else {
        seq = 0b0001 << (stepIndex % 4); // Rotate left
    }
    performStep(seq);
}

// Function to turn the motor by a specified number of steps
void turnSteps(int steps, int interval, char direction) {
    for (int i = 0; i < steps; i++) {
        fullStep(i % 4, direction);  // Ensure we cycle through step sequences
        delay(interval);
    }
}
