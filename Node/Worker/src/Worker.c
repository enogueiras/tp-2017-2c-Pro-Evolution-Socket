#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	char* worker_puerto;
	char* ruta_databin;
} t_worker;

t_worker* config;

t_worker *get_config(const char* path);

int main() {
	config = get_config("../../Configuracion");

	return 0;
}

t_worker *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_worker *config = malloc(sizeof(t_worker));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->worker_puerto = config_get_string_value(c, "WORKER_PUERTO");
	config->ruta_databin = config_get_string_value(c, "RUTA_DATABIN");

	return config;
}
