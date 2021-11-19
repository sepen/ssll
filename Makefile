CFLAGS = -D_INTERNAS -D_NO_INTERACTIVO

all: ssll

ssll: ssll.c defines.h analizador.h redireccion.h ejecucion.h analizador.o redireccion.o ejecucion.o
	gcc $(CFLAGS) ssll.c analizador.o redireccion.o ejecucion.o -o ssll
	strip ssll

analizador.o: analizador.c defines.h analizador.h
		gcc -c $(CFLAGS) analizador.c

redireccion.o: redireccion.c defines.h redireccion.h
		gcc -c $(CFLAGS) redireccion.c

ejecucion.o: ejecucion.c defines.h  ejecucion.h
		gcc -c $(CFLAGS) ejecucion.c

clean:
	rm -v *.o ssll ejecucion1 ejecucion2
