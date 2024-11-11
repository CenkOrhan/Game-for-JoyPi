CC=gcc
CFLAGS=-I.
DEPS = mfrc522.h
OBJ = main.o mfrc522.o
LIBS=-lwiringPi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rfid_app: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core rfid_app
