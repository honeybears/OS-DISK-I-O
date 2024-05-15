CC = gcc
CFLAGS = -Wall

# Source files
SRCS = testcase.c disk.c buf.c 

# Target executable name
TARGET = hw2

# Default target


# Rule to build the executable
$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)