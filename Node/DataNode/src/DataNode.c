#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	char* nombre_datanode;
	char* puerto_datanode;
	char* ruta_databin;
} t_dataNode;

t_dataNode* config;

t_dataNode *get_config(const char* path);

int main() {
	config = get_config("../../Configuracion");

	return 0;
}

t_dataNode *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_dataNode *config = malloc(sizeof(t_dataNode));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->nombre_datanode = config_get_string_value(c, "NOMBRE_NODO");
	config->puerto_datanode = config_get_string_value(c, "DATANODE_PUERTO");
	config->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	return config;
}
