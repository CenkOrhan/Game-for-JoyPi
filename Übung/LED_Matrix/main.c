#include "max7219.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // Für die Verwendung von EXIT_FAILURE

// Prototypen für Funktionen, die in anderen Dateien definiert sind
unsigned char* get_pattern(int num);

void select_and_display_pattern(int file, int pattern_num) {
    unsigned char* pattern = get_pattern(pattern_num);
    max7219_set_pattern(file, pattern);
}

int main() {
    int file;
    setup_spi(&file);
    if (file != -1) {  // Verbesserte Überprüfung auf Datei-Fehler
        max7219_init(file);
        int pattern_num = 0;
        char input[100];  // Buffer für die Benutzereingabe

	// Während der Benutzer Zahlen von 1-9 eingibt, zeigt das Programm die entsprechenden Muster an.
    // Wenn der Benutzer die Zahl 100 eingibt, blinken die LEDs dreimal und das Programm endet.
	// Tipp: Entsprechende Funktionen zur Ansteuerung der Matrix sind in max7219.c zu finden
        while (1) {
	    // TODO: Während der Benutzer Zahlen von 1-9 eingibt, zeigt das Programm die entsprechenden Muster an.
        // TODO: Wenn der Benutzer die Zahl 100 eingibt, blinken die LEDs dreimal und das Programm endet.
        // CODE BITTE HIER EINGEBEN
	    // Tipp: Es sollte überprüft werden, ob der Nutzer etwas eingibt oder was er eingeben hat.
	    // Bei Eingaben, welche NICHT in der Aufgabenbeschreibung zu finden sind, sollte eine Fehlermeldung ausgegeben werden.
	    // Tipp: Bei manchen Abfragen ist es sinnvoll die Schleife nicht abzubrechen.  
        }

        close(file);
    } else {
        printf("Failed to initialize SPI device.\n");
        return EXIT_FAILURE; // Beendet das Programm mit einem Fehlerstatus
    }
    return 0;
}