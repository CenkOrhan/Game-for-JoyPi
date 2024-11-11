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

        while (1) {
            printf("Enter a pattern number (1-9) or 100 to exit: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                continue;  // Bei Fehler oder wenn keine Eingabe erfolgt, erneut versuchen
            }

            if (sscanf(input, "%d", &pattern_num) != 1) {
                printf("Invalid input. Please enter a number.\n");
                continue;
            }

            if (pattern_num == 100) {
                max7219_blink(file);  // Lässt die LEDs drei Mal blinken
                max7219_all_off(file); // Schaltet alle LEDs ab
                break;  // Beendet die Schleife, wenn 100 eingegeben wird
            }

            if (pattern_num < 1 || pattern_num > 9) {
                printf("Invalid pattern number. Please select a number between 1 and 9.\n");
                continue;  // Springt zurück zum Anfang der Schleife, ohne den Rest des Codes auszuführen
            }

            select_and_display_pattern(file, pattern_num);
        }

        close(file);
    } else {
        printf("Failed to initialize SPI device.\n");
        return EXIT_FAILURE; // Beendet das Programm mit einem Fehlerstatus
    }
    return 0;
}