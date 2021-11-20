#ifndef EXECUTE_H
#define EXECUTE_H

/* execute
 * 
 * Create the processes to execute "commands" in pipeline and redirect
 * the STD_INPUT and STD_OUTPUT of a process to / from "pipefd"
 * INPUT: a parsed command "orders" and descriptors
 * "pipefd" of the pipe
 * OUTPUT: OK or ERROR
 */
extern int execute (CMD * ordenes, CMD_FD * pipefd);

#endif

