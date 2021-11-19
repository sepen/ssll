#include "main.h"
#include "parse.h"

char line[MAXLINE+1];
char * lineptr;
char avline[MAXLINE+1];
char * avptr =avline ;

CMD cmd;

#define MAXERR 7
int errnum = 0;
char *errstr[MAXERR] = {
   "Sintaxis correcta",
   "Se esperaba una orden",
   "Redireccion de entrada inesperada",
   "Pipeline inesperado", 
   "Redireccion de salida inesperada",  
   "Background inesperado",
   "Orden inesperada"  
};


void parse_ini(void);
int command(int i);
int check(char * ptr);
void getname(char * name);


CMD * analizar (char * s) 
{
   int i, charok, cmdok;
   
   lineptr = s;
   
   parse_ini();

   /*  1 */
   if ( cmdok=command(0) ) cmd.num_ordenes++;
   
   if (cmdok) {
     /* 2 */
     if (check("<")) getname(cmd.fich_entrada);
    
     /* 3 */
     while ( check("|") && cmd.num_ordenes<PIPELINE && cmdok){ 
          if(cmdok = command(cmd.num_ordenes)) cmd.num_ordenes++;
     }
   }
  
   if (cmdok){
     /* 4 */
     if (check(">")) {
       if (check(">")) cmd.es_append = TRUE;
       getname(cmd.fich_salida);
     }
     
     /* 5 */
     if (check("&"))  cmd.es_background = TRUE;
   
     /* 6 */
     if (check("\n")) return(&cmd);
   }
   if (cmd.num_ordenes==0) return (&cmd);
 
   fprintf(stderr,PROMPT);
   charok=lineptr-line;
   for(i=0; i<charok; i++) fprintf(stderr," ");
   fprintf(stderr,"^ inesperado.\n");
        
   if (!cmdok)          errnum=1;
   else if (check("<")) errnum=2;
   else if (check("|")) errnum=3;
   else if (check(">")) errnum=4;
   else if (check("&")) errnum=5;
   else errnum=6;
   fprintf(stderr, "Error de sintaxis en orden: %s.", cmd.argumentos[cmd.num_ordenes-1][0]);
   fprintf(stderr, " %s\n", errstr[errnum]);
   return (NULL);
}

void parse_ini(void)
{
  int i,j;

   avptr=avline;
   
   for(i=0; i<PIPELINE; i++) 
       for(j=0; j<MAXARG; j++) cmd.argumentos[i][j]=NULL;
   cmd.num_ordenes=0;
   for(i=0; i<PIPELINE; i++) cmd.num_argumentos[i]=0;
   cmd.fich_entrada[0]='\0';
   cmd.fich_salida[0]='\0';
   cmd.es_background = FALSE;
   cmd.es_append = FALSE;
   
   errnum=0;
}


int command(int i) 
{
  int j, flag, inword=FALSE, incmd=FALSE;
  
  cmd.num_argumentos[i]=0;
  for(j=0; j<MAXARG-1; ++j) {
      while(*lineptr==' ' || *lineptr=='\t')  ++lineptr;
      
      cmd.argumentos[i][j] = avptr;
      cmd.argumentos[i][j+1] = NULL;

      for(flag=0; flag==0;) {
         switch(*lineptr) {
         case '>':
         case '<':
         case '|':
         case '&':
         case '\n':
	        if (inword==FALSE) cmd.argumentos[i][j]= NULL;
                *avptr++ = '\0';
                return incmd;
         case ' ':
         case '\t':
	        inword = FALSE;
                *avptr++ = '\0';
		flag = 1 ;
                break;
        default:
               inword = TRUE;
	       incmd=TRUE;
               *avptr++ = *lineptr++;
               break;
       }
       cmd.num_argumentos[i] = j+1;
     }       
  }
  return incmd;
}    

int check(char * ptr)
{
   char *tptr;
   
   while (*lineptr==' ') lineptr++;
   tptr = lineptr;
   
   while (*ptr!='\0' && *ptr==*tptr) {
       ptr++;
       tptr++;
   }
   
   if (*ptr!='\0') return (FALSE);
   else {
         lineptr = tptr;
         return (TRUE);
   }
}

void getname(char * name)
{
   int i;
   
   while (*lineptr==' ') lineptr++;
   for (i=0; i<MAXNAME; ++i) {
       switch (*lineptr) {
        case ' ':
        case '>':
        case '<':
        case '|':
        case '&':
        case '\n':
        case '\t':
                *name = '\0';
                return;
        default:
                *name++ = *lineptr++;
                break;
       }
       *name = '\0';
   }
}

char *error_sintactico ()
{
  return errstr[errnum];
}

void visualizar(CMD *orden)
{  
  int i,j,n,m;
      
  n=orden->num_ordenes;
  fprintf(stdout, "   # ordenes: %d \n",  n);

  for (i=0; i<n;i++){
	 m=orden->num_argumentos[i];
     fprintf(stdout, "\n   nargs[%d]: %d \n", i, m);
     for (j=0; j<m; j++){ 
         fprintf(stdout, "   args[%d,%d]: %s \n", i, j,orden->argumentos[i][j]);
	 }
  }
  fprintf(stdout, "\n   infile: %s \n",  orden->fich_entrada);
  fprintf(stdout, "   outfile: %s \n",  orden->fich_salida);
  fprintf(stdout, "   append: %d \n", orden->es_append);
  fprintf(stdout, "   bgnd: %d \n",  orden->es_background);
  fflush(stdout);
}


