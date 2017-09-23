#include "Master.h"

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

	protocol_handshake_send(yamaSocket);
	protocol_handshake_send(workerSocket);
	char command[BUFFER_CAPACITY];
	while (true) {
		char *argument = input(command);
		if(streq(command, "run")){
			start_job(argument, yamaSocket, workerSocket);
		}
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

void start_job(string path, socket_t yama, socket_t worker){
	char job[BUFFER_CAPACITY];
	ssize_t fsize = readfile(path,job);

	if(fsize == -1){
		printf("Error al leer el archivo. Intente nuevamente.\n");

	}
	//Armar paquete y enviarlo
	socket_send_string(job, yama);
	socket_send_string(job, worker);
}
