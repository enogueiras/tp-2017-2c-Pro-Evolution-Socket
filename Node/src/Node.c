#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

typedef struct{
    char* fs_ip;
    char* fs_puerto;
    char* nombre_nodo;
    char* worker_puerto;
    char* datanode_puerto;
    char* ruta_databin;
}t_node;

t_node* config;

t_node *get_config(const char* path);

int main() {
    config = get_config("../Configuracion");

    return 0;
}

t_node *get_config(const char *path) {
	t_config* c = config_create((char *)path);
	t_node *config = malloc(sizeof(t_node));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->nombre_nodo = config_get_string_value(c, "NOMBRE_NODO");
	config->worker_puerto = config_get_string_value(c, "WORKER_PUERTO");
	config->datanode_puerto = config_get_string_value(c, "DATANODE_PUERTO");
	config->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	return config;
}
