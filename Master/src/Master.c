#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/config.h>
#include "socket.h"
#include "globals.h"

typedef struct {
	char* yama_ip;
	char* yama_puerto;
} t_master;

t_master* config;

t_master *get_config(const char* path);
void conexionConYama();

#define PACKAGESIZE 1024

int main() {

	set_current_process(MASTER);
	title("Master");
	config = get_config("../Configuracion");

	title("Conexiones");
	conexionConYama();

	return 0;

}

void conexionConYama() {
	socket_t yamaSocket = socket_init(config->yama_ip, config->yama_puerto);
	socket_t workerSocket = socket_init(config->yama_ip, "5050");
	printf(
			"Conectado al servidor. Ya puede enviar mensajes. Escriba 'exit' para salir\n");
	int enviar = 1;
	char message[PACKAGESIZE];
	while (enviar) {
		fgets(message, PACKAGESIZE, stdin);
		if (!strcmp(message, "exit\n"))
			enviar = 0;
		if (enviar)
			socket_send_string(message, yamaSocket);
			protocol_handshake_send(workerSocket);
			socket_send_string(message, workerSocket);
	}
	socket_close(yamaSocket);
}

t_master *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_master *config = malloc(sizeof(t_master));

	config->yama_ip = config_get_string_value(c, "YAMA_IP");
	config->yama_puerto = config_get_string_value(c, "YAMA_PUERTO");

	title("Configuracion");
	printf("IP YAMA: %s\n", config->yama_ip);
	printf("PUERTO YAMA: %s\n", config->yama_puerto);

	return config;
}
