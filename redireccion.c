/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :  2005	                       |
 |     Autor      :  Jose Vte Beneyto                  |
 |     Asignatura :  SO2                               |
 |     Descripcion:  modulo de redireccion             |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"

CMDFD cmdfd;

int nproc, mxfd;

void redireccion_ini(void);
int r_entrada(char *s);
int r_salida(char *s, int append);


CMDFD * pipeline(CMD * ordenes)
{
   int i, fds[2];

   redireccion_ini(); 
   nproc=ordenes->num_ordenes;
   r_entrada(ordenes->fich_entrada);
   r_salida(ordenes->fich_salida, ordenes->es_append);

   for (i=0; i<nproc-1; ++i){
      if (pipe(fds) == -1) {  /* crear un tubo */
        fprintf(stderr, "ush: error al intentar crear un pipe\n");
        return FALSE ;
      }
      cmdfd[i].outfd=fds[1];
      cmdfd[i+1].infd=fds[0];
      mxfd=max(mxfd,fds[1]);
   }
   
   if (cmdfd[0].infd==0 && ordenes->es_background)
          cmdfd[0].infd = open ("/dev/null", O_RDONLY); 
   return &cmdfd;

} /* pipeline */

int cerrar_fd()
{
  int i;

  for (i=3;i<=mxfd;i++) close(i);
  return OK;
}

void redireccion_ini()
{
  int i;
  for (i=0; i<PIPELINE; i++){
    cmdfd[i].infd=0;
    cmdfd[i].outfd=1;
  }
  nproc=0;
  mxfd=3;
}

int r_entrada(char *s)
{
  int fd;

  if (s != "\0")
     if ((fd = open(s, O_RDONLY)) == -1) return(-errno);
     else cmdfd[0].infd = fd;
  return(OK);
} /* r_entrada */


int r_salida(char *s, int append)
{
  int fd;

  if (s != "\0"){
    if (append){ 
       if((fd = open(s, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1) return(-errno);
    } else {  
       if((fd = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) return(-errno);
    }
    cmdfd[nproc-1].outfd = fd;
    return(OK);
  }
  return(ERROR);
 } /* r_salida */
