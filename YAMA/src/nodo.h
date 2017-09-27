/*
 * nodo.h
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#ifndef NODO_H_
#define NODO_H_

typedef struct {

	char *nombre;
	char *ip;
	char *puerto;
	int carga;
	int bloque;

}t_nodo;

t_nodo *newNodo(char *nombre,char *ip, char *puerto);

#endif /* NODO_H_ */
