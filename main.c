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
char line[MAX_LINE + 1] = "\0";

// Required to get the command
CMD * command;

// Required to get the redirection
CMD_FD *pipe_fd;

// Required to handle signals
struct sigaction act;

// Function to execute external command for non-interactive mode
void external_command(void)
{
  int pid;

  if ((command = parse(line)) != NULL) {
    if ((pid = fork()) != 0) {
      // Parent
      if (!command->flag_background)
        // Wait for children
        while ((wait(NULL) != pid));
    }
    else {
      // Child
      if (!command->flag_background){
        act.sa_handler = SIG_IGN;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGQUIT, &act, NULL);
      }
      // Handle redirection
      pipe_fd = pipeline(command);
      // Execute command
      execute(command, pipe_fd);
    }
  }
  exit(0);
}

// Function to get the standard input
char * get_newline(void)
{
  int i;

  // Show the prompt string
  printf(PROMPT);

  // Read line char by char until \n appears
  for (i=0; (line[i] = getchar()) != '\n' && line[i] != EOF && i<MAX_LINE; ++i);

  // Perform some checks
  if (line[i] == EOF ) {
    fprintf(stderr, "\nERROR: line[i] == EOF\n");
    clearerr(stdin);
    //exit(0);
    return(line);
  }
  if (i == MAX_LINE) {
    fprintf(stderr, "\nERROR: line equal to MAX_LINE\n");
    clearerr(stdin);
    //exit(0);
    return(line);
  }

  // Append \0 at the end of the line
  line[i+1] = '\0';

  return(line);
}

// Main function
//
// - Read from external input (non-interactive mode)
//   or loop for each newline from user (interactive mode)
// - Execute the command
int main(int argc, char * argv[])
{
  int i;
  char * newline;

  act.sa_handler = SIG_IGN;
  sigaction(SIGINT, &act, NULL);   /* CTRL-C */
  sigaction(SIGQUIT, &act, NULL);  /* CTRL-D */
  sigaction(SIGTTOU, &act, NULL);  /* background process trying to read */
  sigaction(SIGTTIN, &act, NULL);  /* background process trying to write */

  // Welcome message
  printf(MSG_WELCOME);

  // Non-interactive mode
  //
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
  //
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
