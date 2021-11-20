#ifndef REDIRECCION_H
#define REDIRECCION_H

/***********************************************************************/
/*                      pipeline                                       */
/*---------------------------------------------------------------------*/
/* Crea los tubos necesarios (n-1) para ejecutar n ordenes en tuberia  */
/* ENTRADA: una orden analizada (estructura CMD)                       */
/* SALIDA: estruct. CMD_FD con los desc. fd de cada orden de la tuberia */
/*                                                                     */
/***********************************************************************/
extern CMD_FD * pipeline(CMD * ordenes);

/***********************************************************************/
/*                       cerrar_fd                                     */
/*---------------------------------------------------------------------*/
/* Cierra los descriptores fd de los tubos creados con pipeline        */
/* ENTRADA: void                                                       */
/* SALIDA: OK o ERROR                                                  */
/*                                                                     */
/* USO:                                                                */
/* * Cada PROCESO DE UNA TUBERIA: debe cerrarlos, despues de haber     */
/* redirijido su entrada y su salida estandar hacia los tubos          */
/* correspondientes.                                                   */
/*                                                                     */
/* * El PADRE de los procesos de una tuberia tambien debe cerrarlos    */
/* despues de haber creado los hijos.                                  */
/*                                                                     */
/* IMPORTANTE:                                                         */
/* Es imprescindible cerrar todos los descriptores de fichero creados  */
/* o heredados de pipeline que no van a utilizarse.                    */
/* Mientras no se cierren todos los fd de escritura sobre el tubo,     */
/* el lector del tubo no recibira EOF y permanecera esperando mas datos*/ 
/***********************************************************************/
extern int cerrar_fd(void);

#endif

