#CFLAGS = -O2
CFLAGS = -Wall

CC=gcc
STRIP=strip

all: ssll

ssll: main.c main.h parse.h redirect.h execute.h parse.o redirect.o execute.o
	$(CC) $(CFLAGS) main.c parse.o redirect.o execute.o -o ssll
	$(STRIP) ssll

parse.o: main.h parse.h parse.c
	$(CC) -c $(CFLAGS) parse.c

redirect.o: main.h redirect.h redirect.c
	$(CC) -c $(CFLAGS) redirect.c

execute.o: main.h execute.h execute.c
	$(CC) -c $(CFLAGS) execute.c

clean:
	@rm -vf *.o main
