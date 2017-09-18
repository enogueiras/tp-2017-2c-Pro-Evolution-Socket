#include "DataNode.h"

int main() {

	set_current_process(DATANODE);

	title("Data Node");

	config = get_config("../../Configuracion");

	title("Conexiones");
	fsfd = socket_connect(config->fs_ip, config->fs_puerto);

	protocol_handshake_send(fsfd);

	printf("Conectado al FileSystem en %s:%s\n", config->fs_ip, config->fs_puerto);

	while(true) {
		packet_t packet_setBlock = protocol_packet_receive(fsfd);
		setBlock(packet_setBlock.header.msgsize, packet_setBlock.payload);
	}
	return 0;
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

void setBlock(int tamanio, unsigned char* buffer){

	FILE* file = fopen(config->ruta_databin,"rb+");
	int bloque;
	char data[tamanio-2];
	char* longData = string_from_format("h%s%s",string_itoa(tamanio-2),"s");
	serial_unpack(buffer,longData,&bloque,&data);
	if(file){
		fseek(file,0,SEEK_SET);
		fwrite(data,tamanio-2,1,file);
	}

	fclose(file);
}
