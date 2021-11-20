#ifndef PARSE_H
#define PARSE_H

/* parse syntax
 *
 * cmd [ < filename ] [ | cmd ]... [ (> OR >>) filename ] [ & ]
 *
 * Parse a command according to the syntax specified above
 *  INPUT: a string with a command to the shell
 *  OUTPUT: the parsed structure CMDARG
 *          Returns NULL if the command syntax is incorrect
 *          On error, invoking syntactic_error () get a description of
 *          the error
 */

extern CMD * parse (char *s);
extern char *error_sintactico ();

/****************************************************************/
/*                       visualizar                             */
/*--------------------------------------------------------------*/
/* Visualiza los distintos args de la orden analizada     */
/* Util para depuracion                                         */
/****************************************************************/
extern void visualizar (CMD *orden);

#endif
