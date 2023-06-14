# Create a make file for the target read.c


# Variables
CC = gcc
CFLAGS = -Wall -Werror -g
TARGET = read
OBJS = read.o

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

read.o: read.c
	$(CC) $(CFLAGS) -c read.c

clean:
	rm -f $(TARGET) $(OBJS)
