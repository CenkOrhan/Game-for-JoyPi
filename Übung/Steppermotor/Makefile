CC=gcc
CFLAGS=-Wall
LDFLAGS=-lwiringPi

# Name of the executable program
TARGET=stepper_app

# Source files
SOURCES=main.c stepper.c

# Object files
OBJECTS=$(SOURCES:.c=.o)

# Generate the executable program
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule to build object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the project
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
