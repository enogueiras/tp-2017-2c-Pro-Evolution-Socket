#ifndef DATANODE_H_
#define DATANODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "protocol.h"
#include "serial.h"
#include "socket.h"
#include "globals.h"

/*VARIABLES*/

socket_t fsfd;

t_dataNode* config;

/*ESTRUCTURAS*/

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	char* nombre_datanode;
	char* puerto_datanode;
	char* ip_datanode;
	char* ruta_databin;
} t_dataNode;

/*FUNCIONES*/

t_dataNode *get_config(const char* path);

void setBlock(int,unsigned char*);


#endif /* DATANODE_H_ */
