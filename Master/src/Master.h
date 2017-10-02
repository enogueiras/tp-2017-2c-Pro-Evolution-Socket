#ifndef MASTER_H_
#define MASTER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "socket.h"
#include "globals.h"
#include "protocol.h"
#include "utils.h"

typedef struct {
	char* yama_ip;
	char* yama_puerto;
} t_master;

t_master* configMASTER;

t_master *get_config(const char* path);
void conexionConYama();
void start_job(string, socket_t, socket_t);

#define PACKAGESIZE 1024


#endif /* MASTER_H_ */
