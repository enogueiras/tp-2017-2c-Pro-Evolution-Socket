#include "tarea.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int tareaEsTransformacion(t_tarea *tarea)
{
	return tarea->tipo == TAREA_TRANSFORMACION;
}


int tareaEsReduccionLocal(t_tarea *tarea)
{
	return tarea->tipo == TAREA_REDUCCION_LOCAL;
}

int tareaEsReduccionGlobal(t_tarea *tarea)
{
	return tarea->tipo == TAREA_REDUCCION_GLOBAL;
}

void tareaMarcarEnEjecucion(t_tarea *tarea, t_nodo *nodo ,char *nombreResultadoTemporal)
{
	tarea->estado = TAREA_EN_EJECUCION;
	tarea->nodo = nodo;
	tarea->nombreResultadoTemporal = strdup(nombreResultadoTemporal);
}

int tareaEstaEnEjecucion(t_tarea *tarea)
{
	return tarea->estado == TAREA_EN_EJECUCION;
}

int tareaEstaNoEjecutada(t_tarea *tarea)
{
	return tarea->estado == TAREA_NO_EJECUTADA;
}

void tareaMarcarNoEjecutada(t_tarea *tarea)
{
	tarea->estado = TAREA_NO_EJECUTADA;
}

void tareaMarcarFinalizada(t_tarea *tarea)
{
	tarea->estado = TAREA_FINALIZADA;
}

int tareaEstaFinalizada(t_tarea *tarea)
{
	return tarea->estado == TAREA_FINALIZADA;
}

t_nodo *tareaObtenerNodoQueEjecuto(t_tarea *tarea)
{
	return tarea->nodo;
}

char *tareaObtenerNombreResultadoTemporal(t_tarea *tarea)
{
	return tarea->nombreResultadoTemporal;
}
