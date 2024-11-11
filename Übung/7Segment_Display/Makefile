CC=gcc
CFLAGS=-Wall
LDFLAGS=-lrt

# Definiere den Namen der ausführbaren Datei
TARGET=app

# Definiere die Quelldateien
SOURCES=main.c ht16k33.c
HEADERS=ht16k33.h

# Generiere automatisch Objektdateinamen aus den Quelldateinamen
OBJECTS=$(SOURCES:.c=.o)

# Standardziel
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Zum Generieren der einzelnen Objektdateien
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Bereinige die kompilierten Dateien und Editor-Backups
clean:
	rm -f $(TARGET) $(OBJECTS) *~

.PHONY: all clean
