CC=# hier schreibst du den Namen des Compilers
CFLAGS=-Wall
LDFLAGS=# schreibe den Namen der Bibliothek von WiringPi auf um zu linken

TARGET1=#hier schreibst du den Filenamen des Buzzers
TARGET2=# hier schreibst du den Filenamen des Games

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c $(LDFLAGS)

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c $(LDFLAGS)

clean:
	rm -f $(TARGET1) $(TARGET2)

.PHONY: all clean

#Mit Hilfe eins Makefiles mehrere Daten in einem Ordner verknüpfen 
#Schreibe make buzzer oder make buttonGame um die Kompilierung durchzuführen
