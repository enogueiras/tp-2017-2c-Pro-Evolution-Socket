#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

typedef struct {
	char* fs_ip;
	char* fs_puerto;
	int retardo_planif;
	char* algoritmo;
} t_yama;

t_yama* config;

t_yama *get_config(const char* path);

int main() {
	config = get_config("../Configuracion");

	return 0;
}

t_yama *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_yama *config = malloc(sizeof(t_yama));

	config->fs_ip = config_get_string_value(c, "FS_IP");
	config->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	config->retardo_planif = config_get_int_value(c, "RETARDO_PLANIFICACION");
	config->algoritmo = config_get_string_value(c, "ALGORITMO_BALANCEO");

	return config;
}
