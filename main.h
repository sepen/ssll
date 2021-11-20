#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define max(a,b) (((a)>(b)) ? (a): (b))
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define MSG_WELCOME "SepenShell (ssll) 2002-2021\nPress Ctrl+D to exit\n\n"
#define MSG_ERROR "\nUnknown error"
#define PROMPT "\033[1;34m[\033[1;36mssll\033[1;34m]>\033[0m "
#define MAX_LINE 200
#define MAX_ARG 20
#define PIPELINE 10
#define MAX_NAME 100

typedef struct
{
  char * args[PIPELINE][MAX_ARG];
  int cmd_count;
  int args_counts[PIPELINE];
  char file_in[MAX_NAME+1];
  char file_out[MAX_NAME+1];
  int flag_background;
  int flag_append;
} CMD;

typedef struct
{
  int fd_in;
  int fd_out;
} CMD_FD[PIPELINE-1];

#endif
