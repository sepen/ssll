/*------------------------------------------------------+
 |	U S H. C                                        |
 +------------------------------------------------------+
 |	Autor      :  Beneyto Ruiz, Jose Vicente        |
 |	Asignatura :  SO2                               |
 |	Descripcion:  Practica MicroShell               |
 |	Contacto   :  http://www.mikeux.tk              |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>

// Declaracion de variables 
char line[MAXLINE+1] = "\0";
CMD * ordenes;			// necesario para obtener la linea
CMDFD * pipefd;			// necesario para la redireccion
struct sigaction act;

// Declaracion de funciones
char * getline();
//void visualizar_tmp( CMD * orden );	// funcion de uso temporal (practica2)
void modo_externo(void);

// Principal
int main(int argc, char * argv[])
{
	int i, pid, modo;
	char * linea;
	          
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);   /* senyal interrupcion del teclado CTRL-C */
	sigaction(SIGQUIT, &act, NULL);  /* senyal terminacion del teclado CTRL-\*/
	sigaction(SIGTTOU, &act, NULL);  /* proceso background intentando leer */
	sigaction(SIGTTIN, &act, NULL);  /* proceso background intentando escribir */
	
	// Bienvenida
	printf(LOGO);
	
	// **posibilidad**
	// Comandos externos
	if ((argc>1) && (!strcmp(argv[1],"-c"))) {
		for(i=2; i<argc; i++) {
			strcat(line, argv[i]);
			strcat(line, " ");
		}
		strcat(line, "\n");
		modo_externo();
	}
	else {
		while (1) {
			// LEER LINEA
			linea = getline();
		
			// ANALIZAR LINEA
			if ((ordenes=analizar(linea)) != NULL) {
				// LLAMAR A POSIBLES REDIRECCIONES
				pipefd=pipeline(ordenes);
				// EJECUTAR LAS ORDENES
				ejecutar(ordenes, pipefd);
			}	
		}
		return 0;
	}
}
void modo_externo(void) {
	int i,pid;
	if ((ordenes=analizar(line)) != NULL) {
		if ( (pid= fork()) != 0) {  // padre
			if (!ordenes->es_background)
				while ((wait(NULL) != pid));
		} 
		else {  // hijo
			if (!ordenes->es_background){
				act.sa_handler = SIG_IGN;
				sigaction(SIGINT, &act, NULL);
				sigaction(SIGQUIT, &act, NULL);
			}
			// redirecciones
			pipefd=pipeline(ordenes);
			// ejecucion
			ejecutar(ordenes, pipefd);
		}
	}
	exit(0);
}


// Funcion getline (retorno de stdin como cadena)
char * getline(void)
{
	int i;
	
	// Mostrar el prompt
	printf(PROMPT);
	
	for (i=0; (line[i] = getchar()) != '\n' && line[i] != EOF && i<MAXLINE; ++i);
	
	if (line[i] == EOF ) {
		fprintf(stderr, "\n\033[1;34mHasta luego!!\033[0m\n");       
		clearerr(stdin);
		exit(0);
	}
        if (i==MAXLINE) {
		fprintf(stderr, "\nLinea de orden demasiado larga!!\n");
		i=-1;
	}
	line[i+1] = '\0';
	return(line);
}

// Funcion de uso temporal
/*void visualizar_tmp( CMD * orden )
{
	int i,j,n,m;
	n = orden->num_ordenes;
	fprintf(stdout, "\nNº Ordenes= %d \n",  n);
	for (i=0; i<n;i++){
		m = orden->num_argumentos[i];
		fprintf(stdout, "\n--------------------\n   narg[%d]: %d \n", i, m);
		for (j=0; j<m; j++){
			fprintf(stdout, "   arg[%d,%d]: %s \n", i, j, orden->argumentos[i][j]);
		}
	}
	fprintf(stdout, "\n   infile: %s \n",  orden->fich_entrada);
	fprintf(stdout, "   outfile: %s \n",  orden->fich_salida);
	fprintf(stdout, "   append: %d \n", ordenes->es_append);
	fprintf(stdout, "   bgnd: %d \n\n",  ordenes->es_background);
	fflush(stdout);
}*/
