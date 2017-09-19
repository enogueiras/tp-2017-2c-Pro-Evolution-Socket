#include "DataNode.h"

int main() {

	set_current_process(DATANODE);
	log_dataNode = log_create("../Log", "DataNode", true, LOG_LEVEL_INFO);
	title("Data Node");

	config = get_config("../../Configuracion");

	title("Conexiones");
	fsfd = socket_connect(config->fs_ip, config->fs_puerto);

	protocol_handshake_send(fsfd);

	printf("Conectado al FileSystem en %s:%s\n", config->fs_ip,
			config->fs_puerto);

	while (true) {
		recibirMensajesFileSystem();
	}
	return 0;
}

void recibirMensajesFileSystem() {

	unsigned char buffer[BUFFER_CAPACITY];
	packet_t packet = protocol_packet_receive(fsfd);

	int bloqueId;
	char* datos;
	char data[packet.header.msgsize - 2];
	char* longData;

	switch (packet.header.opcode) {
	case OP_SET_BLOQUE:
		longData = string_from_format("h%s%s",
				string_itoa(packet.header.msgsize - 2), "s");
		serial_unpack(packet.payload, longData, &bloqueId, &data);
		setBloque(bloqueId, data);
		break;
	case OP_GET_BLOQUE:
		serial_unpack(buffer, "h", &bloqueId);
		datos = getBloque(bloqueId);
		header_t header_getBloque_response = protocol_header(
				OP_GET_BLOQUE_RESPONSE);
		header_getBloque_response.msgsize = serial_pack(buffer, "s", datos);
		packet_t packet_getBloque_response = protocol_packet(
				header_getBloque_response, buffer);
		protocol_packet_send(packet_getBloque_response, fsfd);
		break;
	default:
		log_error(log_dataNode, "Mensaje desconocido");
	}
}

void setBloque(int bloqueId, char* datos) {
	FILE* file = fopen(config->ruta_databin, "rb+");
	if (file) {
		fseek(file, bloqueId * MB, SEEK_SET);
		fwrite(datos, string_length(datos), 1, file);
	}
	fclose(file);
}

char* getBloque(int bloqueId) {

	char *datos = malloc(1024 * 1024);
	FILE* file = fopen(config->ruta_databin, "rb+");
	if (file) {
		fseek(file, bloqueId, SEEK_SET);
		fread(datos, 1024*1024, 1, file);
	}
	fclose(file);
	return datos;
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

	title("Configuracion");
	printf("IP FS: %s\n", config->fs_ip);
	printf("PUERTO FS: %s\n", config->fs_puerto);
	printf("IP DATANODE: %s\n", config->ip_datanode);
	printf("NOMBRE DATANODE: %s\n", config->nombre_datanode);
	printf("PUERTO DATANODE: %s\n", config->puerto_datanode);
	printf("RUTA DATABIN: %s\n", config->ruta_databin);

	return config;
}
