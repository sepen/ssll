#ifndef REDIRECCION_H
#define REDIRECCION_H

// pipeline
//
// Create the necessary pipes (n - 1) to execute n commands in the pipeline
// - input: a parsed command
// - output: file descriptors for each pipe command
extern CMD_FD * pipeline(CMD * cmd);

// close_fd
//
// Close the file descriptors of the pipes created with pipeline()
// - input: void
// - output: OK or ERROR
//
// Each process of a pipe:
// - must be closed after having redirected the standard input and output to pipes
// Parent or processes of a pipeline
// - must close processes after the children have been created
//
// IMPORTANT: It is essential to close all the crated file descriptors or inherited
// from pipeline that will not be used.
// As long as not all write file descriptors on the pipe are closed, pipe reader
// will not receive EOF and will wait for more data.
extern int close_fd(void);

#endif

