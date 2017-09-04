#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/config.h>
#include "socket.h"
#include "globals.h"

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	int retardo_planif;
	char* algoritmo;
	char* puerto_yama;
} t_yama;

t_yama* config;

t_yama *get_config(const char* path);
void esperarConexiones();

#define BACKLOG 10			// Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar

int main() {

	set_current_process(YAMA);
	title("YAMA");
	config = get_config("../Configuracion");

	title("Conexiones");
	esperarConexiones();

	return 0;
}

void esperarConexiones() {

	printf("Servidor esperando conexiones...\n");
	int yamaSocket = socket_init(NULL, config->puerto_yama);

	struct sockaddr_in addr; // Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	//Atributos para select
	fd_set master;		// conjunto maestro de descriptores de fichero
	fd_set read_fds;// conjunto temporal de descriptores de fichero para select()
	int fdmax;			// número máximo de descriptores de fichero
	int newfd;			// descriptor de socket de nueva conexión aceptada
	int i;
	int nbytes;

	FD_ZERO(&master);	// borra los conjuntos maestro y temporal
	FD_ZERO(&read_fds);

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(NULL, config->puerto_yama, &hints, &serverInfo);

	bind(yamaSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo); // Ya no lo vamos a necesitar

	listen(yamaSocket, BACKLOG);

	// añadir listener al conjunto maestro
	FD_SET(yamaSocket, &master);
	// seguir la pista del descriptor de fichero mayor
	fdmax = yamaSocket; // por ahora es éste

	char package[PACKAGESIZE];

	//------------Comienzo del select------------
	for (;;) {
		read_fds = master; // cópialo
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(1);
		}
		// explorar conexiones existentes en busca de datos que leer
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // ¡¡tenemos datos!!
				if (i == yamaSocket) {
					// gestionar nuevas conexiones
					addrlen = sizeof(addr);
					if ((newfd = accept(yamaSocket, (struct sockaddr*) &addr,
							&addrlen)) == -1) {
						perror("accept");
					} else {
						FD_SET(newfd, &master); // añadir al conjunto maestro
						if (newfd > fdmax) {
							// actualizar el máximo
							fdmax = newfd;
						}
						printf(
								"Servidor: Nueva conexion de %i en " "socket %d\n",
								inet_ntoa(addr.sin_addr), newfd);
					}
				} else {
					// gestionar datos de un cliente
					if ((nbytes = recv(i, (void*) package, PACKAGESIZE, 0))
							<= 0) {
						// error o conexión cerrada por el cliente
						if (nbytes == 0) {
							// conexión cerrada
							printf("selectserver: socket %d hung up\n", i);
						} else {
							perror("recv");
						}
						close(i);
						FD_CLR(i, &master); // eliminar del conjunto maestro
					} else {
						// tenemos datos de algún cliente
						if (nbytes != 0) {
							printf("%s", package);
						}
					}
				}
			}
		}
	}

	socket_close(yamaSocket);

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
