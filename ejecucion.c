/*-----------------------------------------------------+      
 |     E J E C U C I O N . C                           |
 +-----------------------------------------------------+
 |     Version    :  2005	                       |
 |     Autor      :  joberui			       |
 |     Asignatura :  SO2                               |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/
#include "defines.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <signal.h>

struct sigaction act;

int ejecutar(CMD * ordenes, CMDFD *pipefd)
{
	int i, lastpid;
	
	// senyales
	if (ordenes->es_background)
		act.sa_handler = SIG_IGN;
	else
		act.sa_handler = SIG_DFL;
	
	sigaction(SIGCHLD, &act, NULL);
	
	// comprobar si se lanza en 
	// 	-primer plano:	El shell tendra que ir esperando a que los procesos hijos vayan terminando, 
	// 			hasta que lo haga el que aparecía como ultima orden de la linea. 
	// 	-segundo plano: El shell no tiene por que esperar a que termine ninguno de los procesos creados.
	// 			Ya puede presentar el prompt y leer la siguiente orden de inmediato.
	//
	
	// creacion en cadena de los procesos hijos
	//	ej: "awk | grep | ls"
	//	- 'ush'  crea el proceso 'awk'
	//	- 'awk'  crea el proceso 'grep'
	//	- 'grep' crea el proceso 'ls'
	//
	// tratamiento de las redirecciones
	// 
	// ejecutar la orden
	
	for (i=0; i<(ordenes->num_ordenes); ++i){
		if (lastpid = fork()) {
			// *********** PADRE *****************************
			// para cada hijo (orden a ejecutar)
			if (ordenes->es_background) fprintf(stdout, "[%d]\n", lastpid);
		}
		else {
			// *********** HIJO ******************************
			// redireccion de entrada
			if ((*pipefd)[i].infd != 0) {
				close(0);
				dup((*pipefd)[i].infd);
			}
			// redireccion de salida
			if ((*pipefd)[i].outfd != 1) {
				close(1);
				dup((*pipefd)[i].outfd);
			}
			// por si acaso cerrramos los descriptores sobrantes
			cerrar_fd();
			
			// background
			if (!ordenes->es_background) {
			        act.sa_handler = SIG_DFL;
				sigaction(SIGINT, &act, NULL);
				sigaction(SIGQUIT, &act, NULL);
			}
						
			// ejecutamos la orden
			if (execvp(ordenes->argumentos[i][0], ordenes->argumentos[i]) == -1) {
				fprintf(stderr, "%s no encontrado o no ejecutable\n", ordenes->argumentos[i][0]);
				exit(1);
			}
		}
	}
	// *********** PADRE *****************************
	// por si acaso cerrramos los descriptores sobrantes
	cerrar_fd();
	// esperamos la terminacion del ultimo hijo
	// NOTA: podrian quedar zombies con lo que habria que añadir
	// 	 algun mecanismo para evitarlo
	wait(NULL)!=lastpid;
	return TRUE;      
}
