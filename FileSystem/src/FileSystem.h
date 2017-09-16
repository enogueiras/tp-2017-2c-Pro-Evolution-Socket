#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include "socket.h"
#include "thread.h"
#include "serial.h"
#include "protocol.h"

/*ESTRUCTURAS*/

typedef struct {
	char* puerto_fs;
} t_fileSystem;

typedef struct {
	int index;
	char nombre[255];
	int padre;
}t_directory;

typedef struct{
	int tamanio;
	int tipo;
	t_list* bloques;
}t_arch;

typedef struct{
	char* original;
	char* copia;
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

t_fileSystem* config;
t_log* log_fs;
t_directory directorios[100];

struct {
	thread_t thread;
	t_list* clients;
	bool active;
	bool restore;
	bool stable;
} server;

struct {
	int tamanio;
	int libre;
	t_nodo* nodos;
} nodos_table;

/*FUNCIONES*/

void server_start(t_fileSystem*);

void server_end();

void cli_thread(client_t*);

t_fileSystem *get_config(const char* path);

void inicializarConsola();

int importarArchivo(char*, char*);

void enviarADataNode(char*, int, int);

#endif /* FILESYSTEM_H_ */
