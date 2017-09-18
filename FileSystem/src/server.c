#include "server.h"

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
	server.active = true;
	server.thread = thread_create(srv_thread, c->puerto_fs);
}

void server_end() {
	config->stable = false;
	thread_kill(server.thread);
}

void cli_thread(client_t *client) {
	char package[BUFFER_CAPACITY];
	header_t header;

	header = protocol_handshake_receive(client->socket);
	client->type = header.syspid;
	if(client->type == YAMA && !config->stable) {
		socket_close(client->socket);
		printf("Servidor: Socket %d se cerro por no estar estable\n", client->socket);
		bool getClient(void *nbr) {
			client_t *unCliente = nbr;
			return (client->type == unCliente->type);
		}
		client_t *cliente = list_remove_by_condition(server.clients, getClient);
		free(cliente);
	}
	if(client->type == DATANODE && !config->stable) {
		config->stable = true;
	}

	if(client->type == YAMA && config->stable) {
		while(config->stable) {
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
