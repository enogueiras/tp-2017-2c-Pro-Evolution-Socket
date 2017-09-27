#include "logueo.h"
#include <string.h>
#include <commons/log.h>
#include <pthread.h>
#include <stdarg.h>

static pthread_mutex_t mutex_log;
static char *pathArchivoDeLog;

static void realizarLogueo(t_log_level logLevel, int loguearEnPantalla, char *mensaje);

void setPathArchivoDeLog(char *path)
{
	if (path == NULL)
		pathArchivoDeLog = strdup("../Log_YAMA");
	else
		pathArchivoDeLog = strdup(path);

	pthread_mutex_init(&mutex_log, NULL);
}

void loguear(t_log_level logLevel, char *mensaje, ...)
{
	char mensajeALoguear[1024];

	va_list argptr;
	va_start(argptr, mensaje);
	vsnprintf(mensajeALoguear, 1024, mensaje, argptr);
	va_end(argptr);

	realizarLogueo(logLevel, 0, mensajeALoguear);
}

static void realizarLogueo(t_log_level logLevel, int loguearEnPantalla, char *mensaje)
{
	pthread_mutex_lock(&mutex_log);

	t_log *log = log_create("../Log_YAMA", "YAMA", loguearEnPantalla, LOG_LEVEL_TRACE);

	switch(logLevel)
	{
	case LOG_LEVEL_TRACE:
		log_trace(log, mensaje);
		break;
	case LOG_LEVEL_DEBUG:
		log_debug(log, mensaje);
		break;
	case LOG_LEVEL_INFO:
		log_info(log, mensaje);
		break;
	case LOG_LEVEL_WARNING:
		log_warning(log, mensaje);
		break;
	case LOG_LEVEL_ERROR:
		log_error(log, mensaje);
		break;
	}

	log_destroy(log);

	pthread_mutex_unlock(&mutex_log);
}

void loguearYMostrarEnPantalla(t_log_level logLevel, char *mensaje, ...)
{
	char mensajeALoguear[1024 * 1024];

	va_list argptr;
	va_start(argptr, mensaje);
	vsnprintf(mensajeALoguear, 1024, mensaje, argptr);
	va_end(argptr);

	realizarLogueo(logLevel, 1, mensajeALoguear);
}
