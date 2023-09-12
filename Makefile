
TARGET = ssll
STRIP = strip
LIBS =
CC = gcc
CFLAGS = -O2 -pipe

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS) -Wall $(LIBS) -o $@
	$(STRIP) ssll

clean:
	rm -f $(OBJECTS)
	rm -f ssll
