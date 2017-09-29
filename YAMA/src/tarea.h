#ifndef TAREA_H_
#define TAREA_H_

typedef enum {TAREA_TRANSFORMACION, TAREA_REDUCCION_LOCAL, TAREA_REDUCCION_GLOBAL} TipoTarea;

typedef enum {TAREA_NO_EJECUTADA, TAREA_EN_EJECUCION, TAREA_FINALIZADA} EstadoTarea;


typedef struct {
	t_nodo *nodo;
	char *nombreResultadoTemporal;
	EstadoTarea estado;
	TipoTarea tipo;
	uint32_t id;
} t_tarea;

t_tarea *newTareaTransformacion();

t_tarea *newTareaReduccionLocal();

t_tarea *newTareaReduccionGlobal();

char *tareaObtenerNombreResultadoTemporal(t_tarea *tarea);

int tareaEstaNoEjecutada(t_tarea *tarea);

int tareaEstaEnEjecucion(t_tarea *tarea);

int tareaEstaFinalizada(t_tarea *tarea);

void tareaMarcarEnEjecucion(t_tarea *tarea, t_nodo *nodo ,char *nombreResultadoTemporal);

void tareaMarcarNoEjecutada(t_tarea *tarea);

void tareaMarcarFinalizada(t_tarea *tarea);

int tareaEsTransformacion(t_tarea *tarea);

int tareaEsReduccionGlobal(t_tarea *tarea);

int tareaEsReduccionLocal(t_tarea *tarea);

#endif /* TAREA_H_ */
