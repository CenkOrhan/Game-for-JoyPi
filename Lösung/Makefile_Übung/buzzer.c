#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h> // Für sleep-Funktion
#include "buzzer.h" // Inkludiert den Header für den Buzzer

void initializeBuzzer(void) {
    pinMode(BUZZER_PIN, OUTPUT);
}

void activateBuzzer(int duration) {
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer an
    sleep(duration);                // Warte für die angegebene Dauer
    digitalWrite(BUZZER_PIN, LOW);  // Buzzer aus
}

int main(void) {
    if (wiringPiSetup() == -1) {
        printf("Setup wiringPi fehlgeschlagen!\n");
        return 1; // Beendet das Programm mit Fehler
    }

    initializeBuzzer();
    activateBuzzer(1); // Aktiviert den Buzzer für 1 Sekunden

    return 0; // Beendet das Programm erfolgreich
}
