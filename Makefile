#CFLAGS = -O2

all: ssll

ssll: main.c main.h parse.h redirect.h execute.h parse.o redirect.o execute.o
	gcc $(CFLAGS) main.c parse.o redirect.o execute.o -o ssll
	strip ssll

parse.o: main.h parse.h parse.c
	gcc -c $(CFLAGS) parse.c

redirect.o: main.h redirect.h redirect.c
	gcc -c $(CFLAGS) redirect.c

execute.o: main.h execute.h execute.c
	gcc -c $(CFLAGS) execute.c

clean:
	rm -vf *.o main
