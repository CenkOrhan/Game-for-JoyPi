// buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

// Die WiringPi Pin-Nummer, an die der Buzzer angeschlossen ist.
// Achten Sie darauf, diese Nummer entsprechend Ihrer Verkabelung anzupassen.
#define BUZZER_PIN 1 

// Funktion zum Initialisieren des Buzzers.
void initializeBuzzer(void);

// Funktion zum Aktivieren des Buzzers für eine spezifische Dauer.
void activateBuzzer(int duration);

#endif // BUZZER_H
