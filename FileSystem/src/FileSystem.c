#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "socket.h"
#include "thread.h"
#include "protocol.h"

typedef struct {
	char* puerto_fs;
} t_fileSystem;

typedef struct {
	thread_t thread;
	socket_t socket;
	process_t type;
} client_t;

t_fileSystem* config;

struct {
	thread_t thread;
	t_list* clients;
	bool active;
	bool stable;
} server;


void server_start(t_fileSystem*);

void server_end(void);

void cli_thread(client_t*);

t_fileSystem *get_config(const char* path);

void inicializarConsola();

int main(void) {

	set_current_process(FS);

	title("File System");

	config = get_config("../Configuracion");
	server_start(config);
	inicializarConsola();
	return EXIT_SUCCESS;
}

void inicializarConsola() {
	char * linea;
	while (true) {
		linea = readline(">");
		add_history(linea);

		char **argument = string_split(linea, " ");

		if (streq(argument[0],"format")){
			puts("Formateo de Filesystem");
		}
		if (streq(argument[0],"rm")){
			if (argument[1] != NULL){
				if (streq(argument[1],"-d")) puts("Eliminar Directorio");
				if (streq(argument[1],"-b")) puts("Eliminar Nodo/Bloque");
			}
			else{
				puts("Eliminar Archivo");
			}
		}
		if (streq(argument[0],"rename")){
			puts("Renombrar archivo o directorio");
		}
		if (streq(argument[0],"mv")){
			puts("Mover archivo o directorio");
		}
		if (streq(argument[0],"cat")){
			puts("Muestra contenido como texto plano");
		}
		if (streq(argument[0],"mkdir")){
			puts("Crea directorio");
		}
		if (streq(argument[0],"cpfrom")){
			puts("Copia archivo local al yamafs");
		}
		if (streq(argument[0],"cpto")){
			puts("Copia archivo local al yamafs");
		}
		if (streq(argument[0],"cpblock")){
			puts("Crea copia de un bloque");
		}
		if (streq(argument[0],"md5")){
			puts("MD5 de un archivo de yamafs");
		}
		if (streq(argument[0],"ls")){
			puts("Lista archivos");
		}
		if (streq(argument[0],"info")){
			puts("Información del Filesystem");
		}
	}
}


t_fileSystem *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_fileSystem *config = malloc(sizeof(t_fileSystem));

	config->puerto_fs = config_get_string_value(c, "PUERTO_FS");

	title("Configuracion");
	printf("PUERTO FS: %s\n", config->puerto_fs);

	return config;
}

void srv_thread(char * port) {

	char *port_str = port;
	socket_t srv_sock = socket_init(NULL, port_str);
	free(port_str);

	server.clients = list_create();

	socket_t cli_sock;
	while(server.active) {
		cli_sock = socket_accept(srv_sock);

		if(cli_sock != -1) {
			client_t *client = alloc(sizeof(client_t));
			client->socket = cli_sock;
			list_add(server.clients, client);
			client->thread = thread_create(cli_thread, client);
		}
	}

	socket_close(srv_sock);
}

void server_start(t_fileSystem *c) {
	server.stable = false;
	server.active = true;
	server.thread = thread_create(srv_thread, c->puerto_fs);
}

void server_end() {
	server.stable = false;
	thread_kill(server.thread);
}

void cli_thread(client_t *client) {
	char package[BUFFER_CAPACITY];
	header_t header;

	header = protocol_handshake_receive(client->socket);
	client->type = header.syspid;
	if(client->type == YAMA && !server.stable) {
		socket_close(client->socket);
		printf("Servidor: Socket %d se cerro por no estar estable\n", client->socket);
		bool getClient(void *nbr) {
			client_t *unCliente = nbr;
			return (client->type == unCliente->type);
		}
		client_t *cliente = list_remove_by_condition(server.clients, getClient);
		free(cliente);
	}
	if(client->type == DATANODE && !server.stable) {
		server.stable = true;
	}

	if(client->type == YAMA && server.stable) {
		while(server.stable) {
			socket_receive_string(package, client->socket);
			printf("%s", package);
			void replyPackage(void *nbr) {
				client_t *unCliente = nbr;
				if (YAMA!= unCliente->type) {
					socket_send_string(package, unCliente->socket);
				}
			}
			list_iterate(server.clients, replyPackage);
		}
	}

}
