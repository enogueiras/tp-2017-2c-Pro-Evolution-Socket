/*
 * job.h
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#ifndef JOB_H_
#define JOB_H_
#include "tarea.h"

typedef struct{
	t_tarea *transformacion;
	t_tarea *reduccion_local;
	t_tarea *reduccion_global;
}t_job;

t_job *newJob();

#endif /* JOB_H_ */
