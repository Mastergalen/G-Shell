CC = gcc

CFLAGS  = -g -Wall

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).o profileParser.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o profileParser.o

$(TARGET).o: main.c profileParser.h
	$(CC) $(CFLAGS) -c main.c

profileParser.o: profileParser.c profileParser.h
	$(CC) $(CFLAGS) -c profileParser.c

clean:
	$(RM) $(TARGET)
