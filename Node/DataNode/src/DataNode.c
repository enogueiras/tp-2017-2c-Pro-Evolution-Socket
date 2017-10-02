#include "DataNode.h"

int main() {

	set_current_process(DATANODE);
	log_dataNode = log_create("../Log", "DataNode", true, LOG_LEVEL_INFO);
	title("Data Node");

	configDATANODE = get_config("../../Configuracion");

	title("Conexiones");
	fsfd = socket_connect(configDATANODE->fs_ip, configDATANODE->fs_puerto);

	protocol_handshake_send(fsfd);
	enviarNombreYTamanioNodo(fsfd);

	printf("Conectado al FileSystem en %s:%s\n", configDATANODE->fs_ip,
			configDATANODE->fs_puerto);

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

	switch (packet.header.opcode) {
	case OP_SET_BLOQUE:
		serial_unpack(packet.payload, "h", &bloqueId);
		memcpy(data, packet.payload+4, packet.header.msgsize);
		setBloque(bloqueId, data);
		memset(data, 0, packet.header.msgsize - 2);
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
		free(datos);
		break;
	default:
		log_error(log_dataNode, "Mensaje desconocido");
	}
}

void setBloque(int bloqueId, char* datos) {
	FILE* file = fopen(configDATANODE->ruta_databin, "rb+");
	if (file) {
		fseek(file, bloqueId * MB, SEEK_SET);
		fwrite(datos, string_length(datos), 1, file);
	}
	fclose(file);
}

char* getBloque(int bloqueId) {

	char *datos = malloc(MB);
	FILE* file = fopen(configDATANODE->ruta_databin, "rb+");
	if (file) {
		fseek(file, bloqueId, SEEK_SET);
		fread(datos, MB, 1, file);
	}
	fclose(file);
	return datos;
}

void enviarNombreYTamanioNodo(socket_t fsfd) {
	unsigned char buffer[BUFFER_CAPACITY];
	int tamanio = configDATANODE->tamanio_datanode;
	char* nombre = configDATANODE->nombre_datanode;
	header_t header_sendNameTam = protocol_header(
			OP_SEND_NAME_TAM_DATANODE);
	header_sendNameTam.msgsize = serial_pack(buffer, "hs", tamanio, nombre);
	packet_t packet_getBloque_response = protocol_packet(
			header_sendNameTam, buffer);
	protocol_packet_send(packet_getBloque_response, fsfd);

}

t_dataNode *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_dataNode *configDATANODE = malloc(sizeof(t_dataNode));

	configDATANODE->fs_ip = config_get_string_value(c, "FS_IP");
	configDATANODE->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	configDATANODE->nombre_datanode = config_get_string_value(c, "NOMBRE_NODO");
	configDATANODE->ip_datanode = config_get_string_value(c, "IP_NODO");
	configDATANODE->tamanio_datanode = config_get_int_value(c, "TAMANIO_DATANODE");
	configDATANODE->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	title("Configuracion");
	printf("IP FS: %s\n", configDATANODE->fs_ip);
	printf("PUERTO FS: %s\n", configDATANODE->fs_puerto);
	printf("IP DATANODE: %s\n", configDATANODE->ip_datanode);
	printf("NOMBRE DATANODE: %s\n", configDATANODE->nombre_datanode);
	printf("TAMANIO_DATANODE: %i\n", configDATANODE->tamanio_datanode);
	printf("RUTA DATABIN: %s\n", configDATANODE->ruta_databin);

	return configDATANODE;
}
