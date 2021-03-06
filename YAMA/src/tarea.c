#include "tarea.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

t_tarea *newTareaTransformacion()
{
	t_tarea *tarea = malloc(sizeof(t_tarea));
	tarea->tipo = TAREA_TRANSFORMACION;
	tarea->estado = TAREA_NO_EJECUTADA;
	tarea->nodo = NULL;

	return tarea;
}

t_tarea *newTareaReduccionLocal()
{
	t_tarea *tarea = malloc(sizeof(t_tarea));
	tarea->tipo = TAREA_REDUCCION_LOCAL;
	tarea->nodo = NULL;
	tarea->estado = TAREA_NO_EJECUTADA;

	return tarea;
}

t_tarea *newTareaReduccionGlobal()
{
	t_tarea *tarea = malloc(sizeof(t_tarea));
	tarea->tipo = TAREA_REDUCCION_GLOBAL;
	tarea->nodo = NULL;
	tarea->estado = TAREA_NO_EJECUTADA;

	return tarea;
}

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
