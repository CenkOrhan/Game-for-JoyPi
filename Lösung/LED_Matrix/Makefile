CC=gcc
CFLAGS=-Wall
LDFLAGS=-lwiringPi

# Ziel des Makefiles definieren (Name des ausführbaren Programms)
TARGET=max7219_app

# Alle Objektdateien, die für das finale Programm benötigt werden
OBJS=main.o max7219.o

# Standard-Build-Target
all: $(TARGET)

# Wie das Ziel gebaut wird
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Wie Objektdateien aus C-Quelldateien erstellt werden
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# 'make clean' zum Entfernen aller Build-Artefakte
clean:
	rm -f $(TARGET) $(OBJS)

# Verhindert, dass Dateien mit dem Namen der Targets fälschlicherweise behandelt werden
.PHONY: all clean
