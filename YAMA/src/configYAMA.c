#include "configYAMA.h"

t_yama *get_configYAMA(const char *path) {
	t_config* c = config_create((char *) path);
	t_yama *configYAMA = malloc(sizeof(t_yama));

	configYAMA->fs_ip = config_get_string_value(c, "FS_IP");
	configYAMA->fs_puerto = config_get_string_value(c, "FS_PUERTO");
	configYAMA->retardo_planif = config_get_int_value(c,
			"RETARDO_PLANIFICACION");
	configYAMA->algoritmo = config_get_string_value(c, "ALGORITMO_BALANCEO");
	configYAMA->puerto_yama = config_get_string_value(c, "PUERTO_YAMA");

	title("Configuracion");
	printf("IP FS: %s\n", configYAMA->fs_ip);
	printf("PUERTO FS: %s\n", configYAMA->puerto_yama);
	printf("PUERTO YAMA: %s\n", configYAMA->puerto_yama);
	printf("ALGORITMO BALANCEO: %s\n", configYAMA->algoritmo);
	printf("RETARDO PLANIFACION: %i\n", configYAMA->retardo_planif);

	return configYAMA;
}
