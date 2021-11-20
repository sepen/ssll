#include "main.h"
#include "parse.h"
#include "execute.h"
#include "redirect.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// '\0' to denote the terminating null character in a character array
char line[MAX_LINE+1] = "\0";

// required to get the command line
CMD * ordenes;

// required to get the redirection
CMD_FD * pipefd;

// required to handle signals
struct sigaction act;

// function to get the standard input
char * stdin_getline(void)
{
	int i;
	
	// Mostrar el prompt
	printf(PROMPT);
	
	for (i=0; (line[i] = getchar()) != '\n' && line[i] != EOF && i<MAX_LINE; ++i);
	
	if (line[i] == EOF ) {
		//fprintf(stderr, "\n\033[1;34mBye\n");
		clearerr(stdin);
		exit(0);
	}
  if (i==MAX_LINE) {
		fprintf(stderr, "\nToo long line\n");
		i=-1;
	}
	line[i+1] = '\0';
	return(line);
}

void modo_externo(void)
{
	int i,pid;

	if ((ordenes=parse(line)) != NULL) {
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
			execute(ordenes, pipefd);
		}
	}
	exit(0);
}

// main function
int main(int argc, char * argv[])
{
	int i; //, pid; //, modo;
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
			if ((ordenes = parse(linea)) != NULL) {
				// LLAMAR A POSIBLES REDIRECCIONES
				pipefd = pipeline(ordenes);
				// EJECUTAR LAS ORDENES
				execute(ordenes, pipefd);
			}	
		}
		return 0;
	}
}
