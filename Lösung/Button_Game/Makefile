CC=gcc

# Compiler flags:
# -Wall : enables all compiler's warning messages
# -I.   : includes the current directory among the directories to be searched for header files
CFLAGS=-Wall -I.

# Linker flags:
# -lwiringPi : links against the wiringPi library, necessary for GPIO manipulation on the Raspberry Pi
LDFLAGS=-lwiringPi

# Object files to be created:
# button_game.o is the object file generated from the button_game.c source file
OBJ=buttonGame.o

# Target executable name:
# button_game is the final executable that will be generated
TARGET=buttonGame

# Default target executed when no arguments are given to make
all: $(TARGET)

# Rule for linking the final executable:
# Depends on the object files (OBJ)
# '$@' is the left side of the colon, which is the target 'button_game'
# '$^' is the right side of the dependencies, which are the object files listed in OBJ
# $(LDFLAGS) are the linker flags specified above
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule for compiling source files to object files:
# '%.o: %.c' is a pattern rule that says how to convert a .c file into a .o file
# '$<' is the first dependency which in this case is a .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean target:
# 'clean' is a phony target because it doesn't represent a file name, 
# but is just a label for the command to clean up files created during compilation
# 'rm -f $(TARGET) $(OBJ)' forcibly removes the executable and object files
clean:
	rm -f $(TARGET) $(OBJ)

# Declares 'all' and 'clean' as phony targets.
# Phony targets are not actual files, they are just labels for the makefile rules
.PHONY: all clean