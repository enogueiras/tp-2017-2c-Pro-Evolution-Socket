#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include "socket.h"
#include "thread.h"
#include "serial.h"
#include "protocol.h"
#include "console.h"

#define MAX_DIRECTORIOS 100

/*ESTRUCTURAS*/

typedef struct {
	char* puerto_fs;
	bool stable;
	bool restore;
	int nodosEstable;
	char* ruta_metadata;
} t_fileSystem;

typedef struct {
	int index;
	char nombre[255];
	int padre;
}t_directory;

typedef struct{
	char* nombre;
	int tamanio;
	char* tipo;
	int padre;
	bool disponible;
	t_list* bloques;
}t_arch;

typedef struct{
	char** original;
	char** copia;
	int bytes;
}t_block;

typedef struct{
	char* nombre;
	int total;
	int libre;
	t_bitarray* bitmap;
}t_nodo;

typedef struct {
	thread_t thread;
	socket_t socket;
	process_t type;
} client_t;

typedef struct {
	int tamanio;
	int libre;
	char** nombre_nodos;
} t_nodos_table;

t_fileSystem* configFS;
t_log* log_fs;
t_list* archivos;
t_directory directorios[MAX_DIRECTORIOS];
t_nodos_table* tablaNodos;
t_nodo* nodos;
FILE *fileDirectorios, *fileNodos, *fileBitmap;

struct {
	thread_t thread;
	t_list* clients;
	bool active;
} server;

/*FUNCIONES*/


t_fileSystem *get_config(const char* path);

int importarArchivo(char*, char*);

void enviarADataNode(char*, int, int, int);


#endif /* FILESYSTEM_H_ */
