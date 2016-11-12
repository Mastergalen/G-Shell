CC = gcc

CFLAGS  = -g -Wall

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).o loadProfile.o inputHandler.o executeCmd.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o loadProfile.o inputHandler.o executeCmd.o

$(TARGET).o: main.c loadProfile.h inputHandler.h executeCmd.h
	$(CC) $(CFLAGS) -c main.c

executeCmd.o: executeCmd.c executeCmd.h
	$(CC) $(CFLAGS) -c executeCmd.c

loadProfile.o: loadProfile.c loadProfile.h
	$(CC) $(CFLAGS) -c loadProfile.c

inputHandler.o: inputHandler.c inputHandler.h
	$(CC) $(CFLAGS) -c inputHandler.c

clean:
	$(RM) $(TARGET)
