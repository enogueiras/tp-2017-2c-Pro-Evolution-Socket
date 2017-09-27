/*
 * logueo.h
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#ifndef LOGUEO_H_
#define LOGUEO_H_

#include <commons/log.h>

void loguear(t_log_level logLevel, char *mensaje, ...);

void setPathArchivoDeLog(char *path);

void loguearYMostrarEnPantalla(t_log_level logLevel, char *mensaje, ...);


#endif /* LOGUEO_H_ */
