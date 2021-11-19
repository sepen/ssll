#include "main.h"
#include "parse.h"
#include "redirect.h"
#include "execute.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// Declaracion de variables 
char line[MAX_LINE+1] = "\0";
CMD * ordenes;			// necesario para obtener la linea
CMD_FD * pipefd;			// necesario para la redirect
struct sigaction act;

// Declaracion de funciones
char * stdin_getline();

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
	printf(MSG_WELCOME);
	
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
			linea = stdin_getline();
		
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
			if (!ordenes->flag_background)
				while ((wait(NULL) != pid));
		} 
		else {  // hijo
			if (!ordenes->flag_background){
				act.sa_handler = SIG_IGN;
				sigaction(SIGINT, &act, NULL);
				sigaction(SIGQUIT, &act, NULL);
			}
			// redirectes
			pipefd=pipeline(ordenes);
			// execute
			ejecutar(ordenes, pipefd);
		}
	}
	exit(0);
}


// Funcion stdin_getline (retorno de stdin como cadena)
char * stdin_getline(void)
{
	int i;
	
	// Mostrar el prompt
	printf(PROMPT);
	
	for (i=0; (line[i] = getchar()) != '\n' && line[i] != EOF && i<MAX_LINE; ++i);
	
	if (line[i] == EOF ) {
		//fprintf(stderr, "\n\033[1;34mBye!\033[0m\n");
		clearerr(stdin);
		exit(0);
	}
  if (i==MAX_LINE) {
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
	n = orden->cmd_count;
	fprintf(stdout, "\nCommand count= %d \n",  n);
	for (i=0; i<n;i++){
		m = orden->args_counts[i];
		fprintf(stdout, "\n--------------------\n   narg[%d]: %d \n", i, m);
		for (j=0; j<m; j++){
			fprintf(stdout, "   arg[%d,%d]: %s \n", i, j, orden->args[i][j]);
		}
	}
	fprintf(stdout, "file_in: %s \n",  orden->file_in);
	fprintf(stdout, "file_out: %s \n",  orden->file_out);
	fprintf(stdout, "flag_append: %d \n", ordenes->flag_append);
	fprintf(stdout, "flag_background: %d \n",  ordenes->flag_background);
	fflush(stdout);
}*/
