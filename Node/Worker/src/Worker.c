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

t_worker* configYAMA;

t_worker *get_config(const char* path);

void server_start(t_worker*);
void cli_thread(client_t *client);

int main() {

	set_current_process(WORKER);

	title("Worker");

	configYAMA = get_config("../../Configuracion");

	server_start(configYAMA);

	return 0;
}

t_worker *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_worker *configYAMA = malloc(sizeof(t_worker));

	configYAMA->fs_ip = config_get_string_value(c, "FS_IP");
	configYAMA->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	configYAMA->worker_puerto = config_get_string_value(c, "WORKER_PUERTO");
	configYAMA->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	title("Configuracion");
	printf("IP FS: %s\n", configYAMA->fs_ip);
	printf("PUERTO FS: %s\n", configYAMA->fs_puerto);
	printf("PUERTO WORKER: %s\n", configYAMA->worker_puerto);
	printf("RUTA DATABIN: %s\n", configYAMA->ruta_databin);

	return configYAMA;
}

void srv_fork(char * port) {

	char *port_str = port;
	socket_t srv_sock = socket_init(NULL, port_str);
	free(port_str);

	socket_t cli_sock;
	while(server.active) {
		cli_sock = socket_accept(srv_sock);
		if(cli_sock != -1) {
			pid_t  pid = fork();
			if (pid == 0){
				printf("Entré \n");
				client_t *client = alloc(sizeof(client_t));
				client->socket = cli_sock;
				list_add(server.clients, client);
				cli_thread(client);
			}else{
				printf("Luke, I'm your father\n");
			}
		}
	}

	socket_close(srv_sock);
}

void server_start(t_worker *c) {
	title("Conexiones");
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
		while(server.active){
			socket_receive_string(package, client->socket);
			printf("%s", package);
		}
	}

}
