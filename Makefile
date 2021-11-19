CFLAGS = -D_INTERNAS -D_NO_INTERACTIVO

all: ush

ush: ush.c defines.h analizador.h redireccion.h ejecucion.h analizador.o redireccion.o ejecucion.o
	gcc $(CFLAGS) ush.c analizador.o redireccion.o ejecucion.o -o ush
	strip ush

analizador.o: analizador.c defines.h analizador.h
		gcc -c $(CFLAGS) analizador.c

redireccion.o: redireccion.c defines.h redireccion.h
		gcc -c $(CFLAGS) redireccion.c

ejecucion.o: ejecucion.c defines.h  ejecucion.h
		gcc -c $(CFLAGS) ejecucion.c

clean:
	rm -v *.o ush ejecucion1 ejecucion2
