#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h> // For sleep functions
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()

// Define your GPIO pin numbers
#define BUTTON_PIN_LEFT 6  
#define BUTTON_PIN_RIGHT 24
#define BUTTON_PIN_UP 25
#define BUTTON_PIN_DOWN 23

void initializeGPIO() {
    if (wiringPiSetup() == -1) {
        printf("Failed to setup WiringPi\n");
        exit(1);
    }

    pinMode(BUTTON_PIN_LEFT, INPUT);
    pinMode(BUTTON_PIN_RIGHT, INPUT);
    pinMode(BUTTON_PIN_UP, INPUT);
    pinMode(BUTTON_PIN_DOWN, INPUT);

    // Enable pull-up resistors
    pullUpDnControl(BUTTON_PIN_LEFT, PUD_UP);
    pullUpDnControl(BUTTON_PIN_RIGHT, PUD_UP);
    pullUpDnControl(BUTTON_PIN_UP, PUD_UP);
    pullUpDnControl(BUTTON_PIN_DOWN, PUD_UP);
}

int main(void) {
    initializeGPIO();
    srand(time(NULL));  // Seed the random number generator

    int score = 0;
    int gameRunning = 1;
    int correctButton;
    int buttonPressed;
    long reactionTime = 5000000;  // Initial reaction time in microseconds

    while (gameRunning) {
        correctButton = rand() % 4;  // Randomly choose a button (0-3)
        switch (correctButton) {
            case 0: printf("Press LEFT button!\n"); buttonPressed = BUTTON_PIN_LEFT; break;
            case 1: printf("Press RIGHT button!\n"); buttonPressed = BUTTON_PIN_RIGHT; break;
            case 2: printf("Press UP button!\n"); buttonPressed = BUTTON_PIN_UP; break;
            case 3: printf("Press DOWN button!\n"); buttonPressed = BUTTON_PIN_DOWN; break;
        }

        long endTime = micros() + reactionTime;
        int correctPress = 0;

        while (micros() < endTime) {
            if (digitalRead(buttonPressed) == 0) {
                correctPress = 1;
                break;
            }
        }

        if (correctPress) {
            printf("Correct!\n");
            score++;
            reactionTime -= 500000; // Decrease reaction time by 0.5 seconds each round
            if (reactionTime < 1000000) { // Ensure a minimum reaction time of 1 second
                reactionTime = 1000000;
            }
            usleep(500000); // Short pause between rounds
        } else {
            printf("Too slow or wrong button!\n");
            printf("Your current score: %d\n", score);
            printf("Continue playing? (1 for yes, 0 for no): ");
            score = 0;
            scanf("%d", &gameRunning);
        }
    }

    return 0;
}