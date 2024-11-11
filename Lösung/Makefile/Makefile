# Define the compiler to use
CC=gcc

# Compiler flags:
# -Wall turns on most compiler warnings
CFLAGS=-Wall

# Linker flags:
# -lwiringPi links against the WiringPi library, used for GPIO pin control on the Raspberry Pi
LDFLAGS=-lwiringPi

# Names of the target executables
TARGET1=buzzer
TARGET2=buttonGame

# Default target built when you run just "make"
# Builds both TARGET1 and TARGET2
all: $(TARGET1) $(TARGET2)

# Rule to build the first target (buzzer). It depends on the source file $(TARGET1).c.
# Uses the compiler to compile the source file into the executable with the same name as the target.
$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c $(LDFLAGS)

# Rule to build the second target (buttonGame). It depends on the source file $(TARGET2).c.
# Similar to the first target, it compiles the source file into an executable.
$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c $(LDFLAGS)

# Target for cleaning up the project.
# Removes the executables, so you can start a clean build.
clean:
	rm -f $(TARGET1) $(TARGET2)

# Special target to declare that 'all' and 'clean' are not files.
.PHONY: all clean
