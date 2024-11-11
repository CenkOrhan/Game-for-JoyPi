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

        // TODO: Schreiben Sie eine switch-Anweisung, um den Benutzer aufzufordern, einen bestimmten Button zu drücken.

        long endTime = micros() + reactionTime;
        int correctPress = 0;

        // TODO: Implementieren Sie eine Schleife, die überprüft, ob der richtige Button innerhalb der vorgegebenen Zeit gedrückt wird.

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
            score = 0; // Reset the score to 0 upon loss
            printf("Your current score: %d\n", score);
            // TODO: Fragen Sie den Benutzer, ob er weiter spielen möchte, und lesen Sie die Eingabe in die Variable 'gameRunning' ein.
        }
    }

    return 0;
}