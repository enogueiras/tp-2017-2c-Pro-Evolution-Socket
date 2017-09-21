

#ifndef YAMA_H_
#define YAMA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "socket.h"
#include "globals.h"
#include "protocol.h"
#include "log.h"

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	int retardo_planif;
	char* algoritmo;
	char* puerto_yama;
	char* puerto_datanode;
	char* ip_datanode;
} t_yama;

typedef struct{
    socket_t clientID;
    unsigned char process;
}client_t;



typedef struct {
	int job;
	int master;
	char * nodo;
	int bloque;
	char* etapa;
	char* archivoTemporal;
	int estado;
} t_tablaEstados;

t_yama* config;

t_yama *get_config(const char* path);
void init_server(socket_t, t_list*);

socket_t fsfd;


#endif /* YAMA_H_ */
