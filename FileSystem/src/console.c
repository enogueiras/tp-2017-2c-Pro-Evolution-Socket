#include "console.h"

void inicializarConsola() {
	char * linea;
	while (true) {
		linea = readline(">");
		add_history(linea);

		char **argument = string_split(linea, " ");

		if(streq(argument[0], "help")){
			show_command_list();
		}else if (streq(argument[0],"format")){
					puts("Formateo de Filesystem");
		} else 	if (streq(argument[0],"rm")){
					if (argument[1] != NULL){
						if (streq(argument[1],"-d")) puts("Eliminar Directorio");
						if (streq(argument[1],"-b")) puts("Eliminar Nodo/Bloque");
					}
					else{
						puts("Eliminar Archivo");
					}
		} else	if (streq(argument[0],"rename")){
					puts("Renombrar archivo o directorio");
		} else	if (streq(argument[0],"mv")){
					puts("Mover archivo o directorio");
		} else	if (streq(argument[0],"cat")){
					puts("Muestra contenido como texto plano");
		} else	if (streq(argument[0],"mkdir")){
					puts("Crea directorio");
		} else	if (streq(argument[0],"cpfrom")){
					importarArchivo(argument[1], argument[2]);
		} else	if (streq(argument[0],"cpto")){
					puts("Copia archivo del yamafs al local");
		} else	if (streq(argument[0],"cpblock")){
					puts("Crea copia de un bloque");
		} else	if (streq(argument[0],"md5")){
					puts("MD5 de un archivo de yamafs");
		} else	if (streq(argument[0],"ls")){
					puts("Lista archivos");
		} else	if (streq(argument[0],"info")){
					puts("Información del Filesystem");
		} else
					puts("Comando no reconocido. Escriba 'help' para ayuda.");
	}
}

void show_command_list() {
	puts("format							- Formateo de Filesystem");
	puts("rm	<path_archivo>					- Eliminar Archivo");
	puts("rm -d	<path_directorio>				- Eliminar Directorio");
	puts("rm -b	<path_archivo> <nro_bloque>			- Eliminar Archivo");
	puts("rename	<path_original> <nombre_final>			- Renombrar archivo o directorio");
	puts("mv	<path_original> <path_final>			- Mover archivo o directorio");
	puts("cat	<path_archivo>					- Muestra contenido como texto plano");
	puts("mkdir	<path_dir>					- Crea directorio");
	puts("cpfrom	<path_archivo_origen> <directorio_yamafs>	- Copia archivo local al yamafs");
	puts("cpto	<path_archivo_yamafs> <directorio_filesystem>	- Copia archivo del yamafs al local");
	puts("cpblock	<path_archivo> <nro_bloque> <id_nodo>		- Crea copia de un bloque");
	puts("md5	<path_archivo_yamafs>				- MD5 de un archivo de yamafs");
	puts("ls	<path_directorio>				- Lista archivos");
	puts("info	<path_archivo>					- Información del Filesystem");
}
