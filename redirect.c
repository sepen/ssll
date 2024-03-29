#include "main.h"
#include "parse.h"
#include "redirect.h"
#include <unistd.h>

CMD_FD cmdfd;

int nproc, mxfd;

void redirect_ini(void);
int r_input(char *s);
int r_output(char *s, int append);

CMD_FD * pipeline(CMD * cmd)
{
    int i, fds[2];

    redirect_ini();
    nproc=cmd->cmd_count;
    r_input(cmd->file_in);
    r_output(cmd->file_out, cmd->flag_append);

    for (i=0; i<nproc-1; ++i){
        if (pipe(fds) == -1) {  /* pipe */
            fprintf(stderr, "ssll: failed to create pipe\n");
            return FALSE ;
        }
        cmdfd[i].fd_out=fds[1];
        cmdfd[i+1].fd_in=fds[0];
        mxfd=max(mxfd,fds[1]);
   }
   
    if (cmdfd[0].fd_in==0 && cmd->flag_background)
        cmdfd[0].fd_in = open ("/dev/null", O_RDONLY);
    return &cmdfd;
}

int close_fd()
{
  int i;

  for (i=3; i<=mxfd; ++i) {
    close(i);
  }
  return OK;
}

void redirect_ini()
{
  int i;

  for (i=0; i<PIPELINE-1; ++i) {
    cmdfd[i].fd_in=0;
    cmdfd[i].fd_out=1;
  }
  nproc=0;
  mxfd=3;
}

int r_input(char *s)
{
  int fd;

  if ( !strcmp(s, "\0") ) {
    if ( (fd = open(s, O_RDONLY)) == -1) {
        return(-errno);
    }
    else {
        cmdfd[0].fd_in = fd;
    }
  }
  return(OK);
}

int r_output(char *s, int append)
{
  int fd;

  if ( !strcmp(s, "\0") ) {
    if ( append ) {
       if((fd = open(s, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1) return(-errno);
    } else {  
       if((fd = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) return(-errno);
    }
    cmdfd[nproc-1].fd_out = fd;
    return(OK);
  }
  return(ERROR);
 }
