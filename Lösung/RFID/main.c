#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mfrc522.h"

volatile bool continue_reading = true;

typedef struct {
    time_t timestamp;
    char operation[10];  // "read" or "write"
    uint8_t data[16];    // Data read or written
} LogEntry;

LogEntry *logEntries = NULL;
int logCount = 0;

void end_read(int sig) {
    continue_reading = false;
    printf("Ctrl+C captured, ending read.\n");
    mfrc522_stop_crypto1();
    mfrc522_antenna_off();
    // Free allocated memory for logs
    free(logEntries);
}

/**
 * Adds a new log entry to the global log array.
 * 
 * @param operation The operation performed ("read" or "write")
 * @param data The data involved in the operation (16 bytes)
 */
void add_log_entry(const char *operation, uint8_t *data) {
    // Increase the size of the logEntries array to accommodate one more LogEntry
    LogEntry *newEntries = realloc(logEntries, (logCount + 1) * sizeof(LogEntry));
    if (newEntries == NULL) {
        // If realloc fails, print an error and return
        printf("Error allocating memory\n");
        return;
    }
    logEntries = newEntries; // Update the logEntries pointer

    // Get the current time and store it in the new log entry
    logEntries[logCount].timestamp = time(NULL);

    // Copy the operation name into the new log entry
    // Ensure it is null-terminated
    strncpy(logEntries[logCount].operation, operation, sizeof(logEntries[logCount].operation) - 1);
    logEntries[logCount].operation[sizeof(logEntries[logCount].operation) - 1] = '\0';

    // Copy the data into the new log entry
    memcpy(logEntries[logCount].data, data, 16);

    // Increment the count of log entries
    logCount++;
}

/**
 * Prints the last 'count' log entries.
 * 
 * @param count The number of log entries to print
 */
void print_last_logs(int count) {
    // Calculate the starting index for printing logs
    int start = logCount - count;
    if (start < 0) start = 0; // Ensure the starting index is not negative

    printf("Last %d logs:\n", count);

    // Iterate over the log entries from the calculated start index
    for (int i = start; i < logCount; i++) {
        // Convert the timestamp to a readable string
        char *timeStr = ctime(&logEntries[i].timestamp);
        timeStr[strlen(timeStr) - 1] = '\0'; // Remove the newline character at the end of ctime output

        // Print the log entry number and details
        printf("%d. %s - %s - Data: ", i + 1 - start, timeStr, logEntries[i].operation);

        // Print each byte of data in hexadecimal format
        for (int j = 0; j < 16; j++) {
            printf("%02X ", logEntries[i].data[j]);
        }
        printf("\n");
    }
}


int main() {
    uint8_t status, uid[5], key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    int backLen;
    uint8_t dataBlock[16];
    uint8_t blockAddr = 8;  // Block address for read/write operations
    char choice;
    int numLogs;

    srand(time(NULL)); // Initialize random number generator

    // Initialize WiringPi and the MFRC522
    mfrc522_init();
    printf("Welcome to the MFRC522 data read/write example\n");
    printf("Press Ctrl-C to stop.\n");

    // Set up signal handler
    signal(SIGINT, end_read);

    while (continue_reading) {
        // Check for card presence
        status = mfrc522_request(PICC_REQIDL, uid);
        if (status == MI_OK) {
            printf("Card detected\n");

            // Get the UID of the card
            status = mfrc522_anticoll(uid);
            if (status == MI_OK) {
                printf("Card read UID: %x,%x,%x,%x\n", uid[0], uid[1], uid[2], uid[3]);

                // Select the detected tag
                mfrc522_select_tag(uid);

                // Authenticate using key
                status = mfrc522_auth(PICC_AUTHENT1A, blockAddr, key, uid);
                if (status == MI_OK) {
                    do {
                        printf("Choose 'r' to read, 'w' to write, 'l' to show logs: ");
                        scanf(" %c", &choice);

                        if (choice == 'r') {
                            // Read block
                            mfrc522_read(blockAddr, dataBlock);
                            printf("Data in block %d: ", blockAddr);
                            for (int i = 0; i < 16; i++) {
                                printf("%02x ", dataBlock[i]);
                            }
                            printf("\n");
                            add_log_entry("read", dataBlock);
                        } else if (choice == 'w') {
                            // Prepare data for write - generate random data
                            for (int i = 0; i < 16; i++) {
                                dataBlock[i] = rand() % 256;
                            }

                            // Write block
                            mfrc522_write(blockAddr, dataBlock);
                            printf("Random data written to block %d\n", blockAddr);
                            add_log_entry("write", dataBlock);

                            // Read back block to verify
                            mfrc522_read(blockAddr, dataBlock);
                            printf("Data in block %d after write: ", blockAddr);
                            for (int i = 0; i < 16; i++) {
                                printf("%02x ", dataBlock[i]);
                            }
                            printf("\n");
                        } else if (choice == 'l') {
                            printf("Enter the number of entries to display (max 20): ");
                            scanf("%d", &numLogs);
                            if (numLogs > 20) numLogs = 20;
                            if (numLogs < 1) numLogs = 1;
                            print_last_logs(numLogs);
                        }

                        // Stop crypto to allow new operations
                        mfrc522_stop_crypto1();
                    } while (choice != 'r' && choice != 'w' && choice != 'l');
                } else {
                    printf("Authentication error\n");
                }
            } else {
                printf("Failed to read UID.\n");
            }
        }
    }

    return 0;
}
