#include "job.h"

t_job *newJob()
{
	t_job *job = malloc(sizeof(t_job));
	job->transformacion = newTareaTransformacion();
	job->reduccion_local = newTareaReduccionLocal();
	job->reduccion_global = newTareaReduccionGlobal();
	return job;
}

static t_tarea *obtenerTareaSiguiente(t_job *job){

	if (tareaEstaNoEjecutada(job->transformacion)){
		return job->transformacion;
	}

	else if(tareaEstaNoEjecutada(job->reduccion_local)){
		return job->reduccion_local;
	}

	else if (tareaEstaNoEjecutada(job->reduccion_global)){
		return job->reduccion_global;
	}

}
