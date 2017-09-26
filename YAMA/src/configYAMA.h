#ifndef CONFIGYAMA_H_
#define CONFIGYAMA_H_

#include "YAMA.h"

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	int retardo_planif;
	char* algoritmo;
	char* puerto_yama;
	char* puerto_datanode;
	char* ip_datanode;
} t_yama;

t_yama* configYAMA;

t_yama *get_configYAMA(const char* path);

#endif /* CONFIGYAMA_H_ */
