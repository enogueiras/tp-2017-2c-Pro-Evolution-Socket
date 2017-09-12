#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "socket.h"
#include "globals.h"
#include "protocol.h"
#include "log.h"

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	int retardo_planif;
	char* algoritmo;
	char* puerto_yama;
	char* puerto_datanode;
	char* ip_datanode;
} t_yama;

typedef struct{
    socket_t clientID;
    unsigned char process;
}client_t;

t_yama* config;

t_yama *get_config(const char* path);
void init_server(socket_t, t_list*);

socket_t fsfd;

int main() {

	set_current_process(YAMA);

	title("YAMA");
	config = get_config("../Configuracion"); // Carga configuración de archivo

	// Me conecto a FileSystem y envío handshake
	fsfd = socket_connect(config->fs_ip, config->fs_puerto);
	protocol_handshake_send(fsfd);

	title("Conexiones");
	t_list *clientes = list_create(); // Creo lista para gestionar clientes (Master)
	init_server(fsfd, clientes); // Inicia servidor con el socket de FileSystem y la lista de clientes

	return 0;
}

void init_server(socket_t fs_fd, t_list *clientes) {

	// Configuración para el servidor, estructuras administrativas
	printf("Servidor esperando conexiones...\n");
	fdset_t read_fds, all_fds = socket_set_create();
	socket_set_add(fs_fd, &all_fds);
	socket_t sv_sock = socket_init(NULL, config->puerto_yama);
	socket_set_add(sv_sock, &all_fds);

	while(true) {
		read_fds = all_fds;
		fdcheck(select(read_fds.max + 1, &read_fds.set, NULL, NULL, NULL)); // Comienza select

		for(socket_t i = 0; i <= all_fds.max; i++) {
			if(!FD_ISSET(i, &read_fds.set)) continue;
			if(i == sv_sock) {
				// Acepto cliente y recibo el Handshake para validar que sea Master
				socket_t cli_sock = socket_accept(sv_sock);
				header_t header = protocol_handshake_receive(cli_sock);
				process_t cli_process = header.syspid;
				if(cli_process == MASTER) { // Si es Master lo agrego a mi lista de clientes
					socket_set_add(cli_sock, &all_fds);
					client_t* cliente = alloc(sizeof(client_t));
					cliente->clientID = cli_sock;
					cliente->process= MASTER;
					list_add(clientes, cliente);
					log_inform("Received handshake from %s", get_process_name(cli_process));
				}
				else { // Si no es Master lo rechazo y cierrp el socket
					printf("Servidor: Socket %d se cerro\n", i);
					socket_close(cli_sock);
				}
			}
			else {// En caso de que no sea un nuevo cliente, recibo el paquete y debo validar si es de FileSystem o de un cliente Master
			      // Sólo está el caso de que sea de Master, esto ven cómo definirlo
					bool getClient(void *nbr) {
						client_t *unCliente = nbr;
						return (unCliente->clientID == i);
					}
					client_t* client = list_find(clientes, getClient);
					packet_t packet = protocol_packet_receive(client->clientID);
					if(client->process == MASTER) {
						//Switch según operación
					}
				}
			}
		}
}


t_yama *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_yama *config = malloc(sizeof(t_yama));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->retardo_planif = config_get_int_value(c, "RETARDO_PLANIFICACION");
	config->algoritmo = config_get_string_value(c, "ALGORITMO_BALANCEO");
	config->puerto_yama = config_get_string_value(c, "PUERTO_YAMA");

	title("Configuracion");
	printf("IP FS: %s\n", config->fs_ip);
	printf("PUERTO FS: %s\n", config->puerto_yama);
	printf("PUERTO YAMA: %s\n", config->puerto_yama);
	printf("ALGORITMO BALANCEO: %s\n", config->algoritmo);
	printf("RETARDO PLANIFACION: %i\n", config->retardo_planif);

	return config;
}
