#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

typedef struct {
	char* yama_ip;
	char* yama_puerto;
} t_master;

t_master* config;

t_master *get_config(const char* path);

int main() {
	config = get_config("../Configuracion");

	return 0;
}

t_master *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_master *config = malloc(sizeof(t_master));

	config->yama_ip = config_get_string_value(c, "YAMA_IP");
	config->yama_puerto = config_get_string_value(c, "YAMA_PUERTO");

	return config;
}
