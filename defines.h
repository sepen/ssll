/*-----------------------------------------------------+
 |     defines . H                                     |
 +-----------------------------------------------------+
 |     Version    :  4-Julio-2002                      |
 |     Autor      :                           |
 |     Asignatura :  SO2                               |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/

#ifndef DEFINES_H
#define DEFINES_H

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

#define LOGO "\n\033[1;34m[uSh] MicroShell (c)2005\033[0m\n"
#define PROMPT "\n\033[1;34m[\033[1;36muSh\033[1;34m]\033[0m "	    

// ---------------------------
#define MSG_ERROR "\nError\n"
// ---------------------------

				    
#define MAXLINE 200
#define MAXARG 20
#define PIPELINE 10
#define MAXNAME 100

typedef struct
{
  char * argumentos[PIPELINE][MAXARG];
  int num_ordenes;
  int num_argumentos[PIPELINE];
  char fich_entrada[MAXNAME+1];
  char fich_salida[MAXNAME+1];
  int es_background;
  int es_append;
} CMD;

typedef struct
{
  int infd;
  int outfd;
}CMDFD[PIPELINE-1];


#endif

