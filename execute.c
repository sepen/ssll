#include "main.h"
#include "redirect.h"
#include "execute.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

struct sigaction act;

int execute(CMD * cmd, CMD_FD *pipefd)
{
	int i, lastpid;
	
	// signals
	if (cmd->flag_background)
		act.sa_handler = SIG_IGN;
	else
		act.sa_handler = SIG_DFL;
	
	sigaction(SIGCHLD, &act, NULL);

	// check if is launched in
	// - foreground: the command should wait until the last child in the pipeline
	// - background: the command shouldn't wait to anything, it can give you a prompt and read a newline
	//
	
	// each process creates a child
	// e.g: "ps | grep | awk"
	// - 'ssll' creates process 'ps'
	// - 'grep' creates process 'grep'
	// - 'awk'  creates process 'awk'

	
	for (i=0; i<(cmd->cmd_count); ++i){
		if ( (lastpid = fork()) ) {
			// --- parent ---
			// for each child
			if (cmd->flag_background) fprintf(stdout, "[%d]\n", lastpid);
		}
		else {
			// --- children ---
			// input redirection
			if ((*pipefd)[i].fd_in != 0) {
				close(0);
				dup((*pipefd)[i].fd_in);
			}
			// output redirection
			if ((*pipefd)[i].fd_out != 1) {
				close(1);
				dup((*pipefd)[i].fd_out);
			}
			// we close the other file descriptors just in case
			close_fd();
			
			// background
			if (!cmd->flag_background) {
			        act.sa_handler = SIG_DFL;
				sigaction(SIGINT, &act, NULL);
				sigaction(SIGQUIT, &act, NULL);
			}
						
			// execute the command
			if (execvp(cmd->args[i][0], cmd->args[i]) == -1) {
				fprintf(stderr, "%s: command not found\n", cmd->args[i][0]);
				exit(1);
			}
		}
	}
	// --- parent ---
	// we close the other file descriptors just in case
	close_fd();
	// wait for the last child to terminate
	// TODO: detect zombies
	while ( (wait(NULL) != lastpid) );
	return TRUE;      
}
