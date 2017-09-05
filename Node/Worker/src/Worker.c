#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "socket.h"
#include "thread.h"
#include "protocol.h"

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	char* worker_puerto;
	char* ruta_databin;
} t_worker;

typedef struct {
	socket_t socket;
	process_t type;
} client_t;

struct {
	thread_t thread;
	t_list* clients;
	bool active;
} server;

t_worker* config;

t_worker *get_config(const char* path);

void server_start(t_worker*);
void cli_thread(client_t *client);

int main() {
	config = get_config("../../Configuracion");

	server_start(config);

	return 0;
}

t_worker *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_worker *config = malloc(sizeof(t_worker));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->worker_puerto = config_get_string_value(c, "WORKER_PUERTO");
	config->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	return config;
}

void srv_fork(char * port) {

	char *port_str = port;
	socket_t srv_sock = socket_init(NULL, port_str);
	free(port_str);

	socket_t cli_sock;
	while(server.active) {
		cli_sock = socket_accept(srv_sock);
		if(cli_sock != -1) {
			pid_t  pid;
			pid = fork();
			if (pid == 0){
				client_t *client = alloc(sizeof(client_t));
				client->socket = cli_sock;
				list_add(server.clients, client);
				cli_thread(client);
			}else{
				continue;
			}
		}
	}

	socket_close(srv_sock);
}

void server_start(t_worker *c) {
	server.active = true;
	server.clients = list_create();
	srv_fork(c->worker_puerto);
}


void cli_thread(client_t *client) {
	char package[BUFFER_CAPACITY];
	header_t header;

	header = protocol_handshake_receive(client->socket);
	client->type = header.syspid;
	if(client->type == MASTER) {
		while(server.active) {
			socket_receive_string(package, client->socket);
			printf("%s", package);
		}
	}

}
