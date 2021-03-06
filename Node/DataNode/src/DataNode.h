#ifndef DATANODE_H_
#define DATANODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "protocol.h"
#include "serial.h"
#include "socket.h"
#include "globals.h"
#include <commons/log.h>

/*ESTRUCTURAS*/

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	char* nombre_datanode;
	int tamanio_datanode;
	char* ip_datanode;
	char* ruta_databin;
} t_dataNode;

/*Variables*/

t_log* log_dataNode;

socket_t fsfd;

t_dataNode* configDATANODE;

/*Funciones*/

void recibirMensajesFileSystem();

t_dataNode *get_config(const char* path);

void setBloque(int bloqueId, char* datos);

char* getBloque(int bloqueId);

void enviarNombreYTamanioNodo(socket_t fsfd);

#endif /* DATANODE_H_ */
