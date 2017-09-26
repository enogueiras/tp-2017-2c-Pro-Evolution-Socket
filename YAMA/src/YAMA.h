

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
#include "serial.h"



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


void init_server(socket_t, t_list*);

socket_t fsfd;


#endif /* YAMA_H_ */
