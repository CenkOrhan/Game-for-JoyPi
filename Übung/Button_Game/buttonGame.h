#ifndef BUTTON_GAME_H
#define BUTTON_GAME_H

#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h> // Für sleep-Funktionen
#include <stdlib.h> // Für rand() und srand()
#include <time.h>   // Für time()

// GPIO-Pin-Nummern
#define BUTTON_PIN_LEFT 6  
#define BUTTON_PIN_RIGHT 24
#define BUTTON_PIN_UP 25
#define BUTTON_PIN_DOWN 23
#define BUTTON_PIN_SPACE 21

// Funktionsdeklarationen
void initializeGPIO();

#endif // BUTTON_GAME_H
