#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "protocol.h"
#include "socket.h"

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	char* nombre_datanode;
	char* puerto_datanode;
	char* ip_datanode;
	char* ruta_databin;
} t_dataNode;

socket_t fsfd;

t_dataNode* config;

t_dataNode *get_config(const char* path);

int main() {
	config = get_config("../../Configuracion");
	char package[BUFFER_CAPACITY];

	printf("Estableciendo conexión con el FileSystem...");
	fsfd = socket_connect(config->fs_ip, config->fs_puerto);

	protocol_handshake_send(fsfd);
	socket_send_string(config->ip_datanode, fsfd);
	socket_send_string(config->puerto_datanode, fsfd);
	printf("\33[2K\rConectado al FileSystem en %s:%s\n", config->fs_ip, config->fs_puerto);

	while(true) {
		socket_receive_string(package, fsfd);
		printf("%s", package);
	}
	return 0;
}

t_dataNode *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_dataNode *config = malloc(sizeof(t_dataNode));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->nombre_datanode = config_get_string_value(c, "NOMBRE_NODO");
	config->ip_datanode = config_get_string_value(c, "IP_NODO");
	config->puerto_datanode = config_get_string_value(c, "DATANODE_PUERTO");
	config->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	return config;
}
