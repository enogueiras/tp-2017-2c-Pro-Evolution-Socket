#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/config.h>
#include "socket.h"

typedef struct {
	char* puerto_fs;
} t_fileSystem;

t_fileSystem* config;

t_fileSystem *get_config(const char* path);

void inicializarConsola(void);

int main(void) {

	config = get_config("../Configuracion");

	inicializarConsola();
	return EXIT_SUCCESS;
}

void inicializarConsola(void) {
	char * linea;
	while (1) {
		linea = readline(">");
		if (linea)
			add_history(linea);
		if (!strncmp(linea, "exit", 4)) {
			free(linea);
			break;
		}
		printf("%s\n", linea);
		free(linea);
	}
}

t_fileSystem *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_fileSystem *config = malloc(sizeof(t_fileSystem));

	config->puerto_fs = config_get_string_value(c, "PUERTO_FS");

	return config;
}
