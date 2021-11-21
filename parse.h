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

extern CMD *parse(char *s);
extern char *syntax_error();
extern void visualize(CMD *command);

#endif