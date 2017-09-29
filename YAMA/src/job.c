#include "job.h"

t_job *newJob()
{
	t_job *job = malloc(sizeof(t_job));
	job->transformacion = newTareaTransformacion();
	job->reduccion_local = newTareaReduccionLocal();
	job->reduccion_global = newTareaReduccionGlobal();
	return job;
}

t_tarea obtenerTareaSiguienteA(t_job job, t_tarea tarea){

	if (tareaEsTransformacion(tarea) && tareaEstaFinalizada(tarea)){
		return job->reduccion_local;
	}

	else if(tareaEsReduccionLocal(tarea) && tareaEstaFinalizada(tarea)){
		return job->reduccion_global;
	}

}
