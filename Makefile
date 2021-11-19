CFLAGS = -D_INTERNAS -D_NO_INTERACTIVO

all: ssll

ssll: ssll.c defines.h parse.h redirect.h execute.h parse.o redirect.o execute.o
	gcc $(CFLAGS) ssll.c parse.o redirect.o execute.o -o ssll
	strip ssll

parse.o: parse.c defines.h parse.h
		gcc -c $(CFLAGS) parse.c

redirect.o: redirect.c defines.h redirect.h
		gcc -c $(CFLAGS) redirect.c

execute.o: execute.c defines.h  execute.h
		gcc -c $(CFLAGS) execute.c

clean:
	rm -v *.o ssll execute1 execute2
