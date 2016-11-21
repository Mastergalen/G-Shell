CC = gcc

CFLAGS  = -g -Wall

TARGET = shell

OBJDIR=bin

OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(wildcard *.c))

all: $(TARGET)

createBinDir:
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c | createBinDir
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) $(TARGET) $(OBJECTS)
