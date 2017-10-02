
#ifndef INICIALIZACION_H_
#define INICIALIZACION_H_

#include "FileSystem.h"

void format_fs(t_fileSystem*,t_directory[]);

void restablecerEstado();

void restablecerNodos(t_config*);

void ingresarDirectorios(char*);

void recuperarArchivos(char*);

#endif /* INICIALIZACION_H_ */
