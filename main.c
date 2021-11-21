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

// required to get the command
CMD * command;

// required to get the redirection
CMD_FD *pipe_fd;

// required to handle signals
struct sigaction act;

// function to execute external command
void external_command(void)
{
	int pid;

	if ((command = parse(line)) != NULL) {
		if ((pid = fork()) != 0) {
		    // parent
			if (!command->flag_background)
				while ((wait(NULL) != pid));
		}
		else {
		    // child
			if (!command->flag_background){
				act.sa_handler = SIG_IGN;
				sigaction(SIGINT, &act, NULL);
				sigaction(SIGQUIT, &act, NULL);
			}
			// handle redirections
			pipe_fd = pipeline(command);
			// execute
			execute(command, pipe_fd);
		}
	}
	exit(0);
}

// function to get the standard input
char * get_newline(void)
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

// main function
//
// reads from external input or loop for each newline from user
int main(int argc, char * argv[])
{
	int i; //, pid, modo;
	char * newline;

	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);   /* CTRL-C */
	sigaction(SIGQUIT, &act, NULL);  /* CTRL-D */
	sigaction(SIGTTOU, &act, NULL);  /* background process trying to read */
	sigaction(SIGTTIN, &act, NULL);  /* background process trying to write */
	
	// Welcome message
	printf(MSG_WELCOME);

	// Read from external arguments and execute the command
	// Example: ssll -c uptime
	if ((argc>1) && (!strcmp(argv[1],"-c"))) {
		for(i=2; i<argc; i++) {
			strcat(line, argv[i]);
			strcat(line, " ");
		}
		strcat(line, "\n");
		external_command();
	}
	// Interactive mode
	// Give a prompt, parse each newline from user and execute the command
	else {
		while (1) {
			// read newline from user
			newline = get_newline();
		
			// parse the line
			if ((command = parse(newline)) != NULL) {
				// get file descriptors for possible redirection
				pipe_fd = pipeline(command);
				// execute the command with file descriptors from above
				execute(command, pipe_fd);
			}	
		}
		return 0;
	}
}
