/*-----------------------------------------------------+
 |     A N A L I Z A D O R . H                         |
 +-----------------------------------------------------+
 |     Version    :  4-Julio-2002                      |
 |     Autor      :                           |
 |     Asignatura :  SO2                               |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/

#ifndef ANALIZADOR_H
#define ANALIZADOR_H

/****************************************************************/
/*                      SINTAXIS                                */
/*--------------------------------------------------------------*/
/*                                                              */
/* cmd [ < filename ] [ | cmd ]... [ (> OR >>) filename ] [ & ] */
/*                                                              */
/****************************************************************/

/****************************************************************/
/*                        analizar                              */
/*--------------------------------------------------------------*/
/* Analiza una orden segun la sintaxis arriba especificada      */
/* ENTRADA: una cadena con una orden para el shell              */
/* SALIDA: la estructura analizada CMDARG                       */
/*         Retorna  NULL si la sintaxis de laorden es incorrecta*/
/*         En caso de error, invocando error_sintactico()       */
/*         se obtiene una descripción del error.                */
/****************************************************************/
extern CMD *  analizar (char *s);
extern char *error_sintactico ();

/****************************************************************/
/*                       visualizar                             */
/*--------------------------------------------------------------*/
/* Visualiza los distintos args de la orden analizada     */
/* Util para depuracion                                         */
/****************************************************************/
extern void visualizar (CMD *orden);

#endif
