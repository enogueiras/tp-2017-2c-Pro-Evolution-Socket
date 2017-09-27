#include "YAMA.h"
#include "configYAMA.h"
#include "logueo.h"

void establecerConexionFS(char * fs_ip,char * fs_puerto) {
	// Me conecto a FileSystem y envío handshake
	loguearYMostrarEnPantalla(LOG_LEVEL_INFO, "Iniciando conexión con File System");
	fsfd = socket_connect(fs_ip, fs_puerto);
	protocol_handshake_send(fsfd);
	loguearYMostrarEnPantalla(LOG_LEVEL_INFO, "Conectado correctamente al File System");
}


int main() {

	set_current_process(YAMA);
	setPathArchivoDeLog("../Log_YAMA");
	title("YAMA");
	configYAMA = get_configYAMA("../Configuracion"); // Carga configuración de archivo

	// Me conecto a FileSystem y envío handshake
	establecerConexionFS(configYAMA -> fs_ip, configYAMA -> fs_puerto);
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
	socket_t sv_sock = socket_init(NULL, configYAMA->puerto_yama);
	socket_set_add(sv_sock, &all_fds);

	while (true) {
		read_fds = all_fds;
		fdcheck(select(read_fds.max + 1, &read_fds.set, NULL, NULL, NULL)); // Comienza select

		for (socket_t i = 0; i <= all_fds.max; i++) {
			if (!FD_ISSET(i, &read_fds.set))
				continue;
			if (i == sv_sock) {
				// Acepto cliente y recibo el Handshake para validar que sea Master
				socket_t cli_sock = socket_accept(sv_sock);
				header_t header = protocol_handshake_receive(cli_sock);
				process_t cli_process = header.syspid;
				if (cli_process == MASTER) { // Si es Master lo agrego a mi lista de clientes
					socket_set_add(cli_sock, &all_fds);
					client_t* cliente = alloc(sizeof(client_t));
					cliente->clientID = cli_sock;
					cliente->process = MASTER;
					list_add(clientes, cliente);
					log_inform("Received handshake from %s",
							get_process_name(cli_process));
				} else { // Si no es Master lo rechazo y cierrp el socket
					printf("Servidor: Socket %d se cerro\n", i);
					socket_close(cli_sock);
				}
			} else { // En caso de que no sea un nuevo cliente, recibo el paquete y debo validar si es de FileSystem o de un cliente Master
					 // Sólo está el caso de que sea de Master, esto ven cómo definirlo
				bool getClient(void *nbr) {
					client_t *unCliente = nbr;
					return (unCliente->clientID == i);
				}
				client_t* client = list_find(clientes, getClient);
				packet_t packet = protocol_packet_receive(client->clientID);
				if (client->process == MASTER) {
					char * longData;
					char * script;
					switch (packet.header.opcode) {
						case OP_SEND_JOB_FILE:
							//Me manda el nombre del archivo a buscar en FS
							longData = string_from_format("h%s%s",
									string_itoa(packet.header.msgsize - 1), "s");
							serial_unpack(packet.payload, longData, &script);
							//Preguntar a FS info sobre el archivo
							//Empezar planificacion y mandar mensajes a MASTER
							break;
						case OP_SEND_JOB_RESULTADO:
							//Acá es la ruta y nombre del archivo final a almacenar
							break;
						case OP_JOB_NODO_OK:
							//Acá me envia un paquete con la informacion del nodo que terminó ok
							//Volver a planificar para este nodo la siguiente etapa
							break;
						case OP_JOB_NODO_ERORR:
							// Me envia paquete informacion sobre nodo que tiró error
							// Fijarse la etapa donde falló y abortar el job o volver a planificar sobre este
							break;
						default:
							break;
					}

				}
			}
		}
	}
}
