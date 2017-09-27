#include "nodo.h"
#include <stdlib.h>
#include <string.h>


t_nodo *newNodo(char *nombre,char *ip, char *puerto)
{
	t_nodo *nodo = malloc(sizeof(t_nodo));
	nodo->nombre = strdup(nombre);
	nodo->ip = strdup(ip);
	nodo->puerto = strdup(puerto);
	nodo->carga = 0;

	return nodo;
}

// Habría que tener una noción de que el nodo tiene una carga y jugar subiendo o bajando distintas tareas como
// transformacion, reduccion, etc..

// Con esta nocion de nodo podríamos planificar
