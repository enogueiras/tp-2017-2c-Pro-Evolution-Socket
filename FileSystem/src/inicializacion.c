#include "inicializacion.h"

void format_fs(t_fileSystem *config,t_directory directorios[MAX_DIRECTORIOS]){

	system(string_from_format("exec rm -r %s%s",config->ruta_metadata,"/*"));
    mkdir(string_from_format("%s%s",config->ruta_metadata,"/archivos/"), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(string_from_format("%s%s",config->ruta_metadata,"/archivos/0/"), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(string_from_format("%s%s",config->ruta_metadata,"/bitmaps/"), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    config->restore = true;
	config->nodosEstable = 2;
	directorios[0].index = 0;
	directorios[0].padre = -1;
	strcpy(directorios[0].nombre, "root");
	for (int i = 1; i < MAX_DIRECTORIOS; i++){
		strcpy(directorios[i].nombre, string_new());
	}

	if (!(fileDirectorios = fopen(string_from_format("%s%s",config->ruta_metadata,"/directorios.dat"), "wb+"))){
		log_error(log_fs, "El archivo no se pudo abrir");
	}else{
		for (int i = 0; i < MAX_DIRECTORIOS; i++){
			fwrite (&directorios[i], sizeof(t_directory),1,fileDirectorios);
		}
	}
	if (!(fileNodos = fopen(string_from_format("%s%s",config->ruta_metadata,"/nodos.bin"), "wb+")))
		log_error(log_fs, "El archivo no se pudo abrir");

}

void restablecerNodos(t_config* restore_config) {
	t_nodos_table* tablaNodos = alloc(sizeof(t_nodos_table));
	tablaNodos->tamanio = config_get_int_value(restore_config, "TAMANIO");
	tablaNodos->libre = config_get_int_value(restore_config, "LIBRE");
	tablaNodos->nombre_nodos= config_get_array_value(restore_config, "NODOS");
	int counter = 0;
	while (tablaNodos->nombre_nodos[counter])
		counter++;
	nodos = realloc(nodos, counter * sizeof(t_nodo));
	for (int i = 0; i < counter; i++) {
		nodos[i].total = config_get_int_value(restore_config,
				string_from_format("%s%s", tablaNodos->nombre_nodos[i],
						"Total"));
		nodos[i].libre = config_get_int_value(restore_config,
				string_from_format("%s%s", tablaNodos->nombre_nodos[i],
						"Libre"));
		if (!(fileBitmap = fopen(
				string_from_format("%s%s%s%s", configYAMA->ruta_metadata,
						"/bitmaps/", tablaNodos->nombre_nodos[i], ".dat"),
				"rb+"))) {
			log_error(log_fs, "El archivo no se pudo abrir");
			format_fs(configYAMA,directorios);
		} else {
			int size_bytes;
			fseek(fileBitmap, 0, SEEK_END);
			size_bytes = ftell(fileBitmap);
			rewind(fileBitmap);

			if(size_bytes == 0){
				format_fs(configYAMA,directorios);
			}else{
				char* map;
				if ((map = mmap(NULL, size_bytes, PROT_READ | PROT_WRITE,
						MAP_SHARED, fileno(fileBitmap), 0)) == MAP_FAILED) {
					log_error(log_fs, "Error al mappear archivo\n");
				} else {
					nodos[i].bitmap = bitarray_create_with_mode(map, nodos[i].total,
							LSB_FIRST);
				}
			}
		}
	}

}

void recuperarArchivos(char* pathArchivos){
	DIR* mydir;
	struct dirent* myfile;
	struct stat mystat;
	mydir = opendir(pathArchivos);
	while ((myfile = readdir(mydir)) != NULL) {
		stat(myfile->d_name, &mystat);
		if (!string_equals_ignore_case(myfile->d_name, "..")
				&& !string_equals_ignore_case(myfile->d_name, ".")) {
			t_config* config_archivo = config_create(
					string_from_format("%s/%s", pathArchivos, myfile->d_name));
			t_arch* dataArch = alloc(sizeof(t_arch));
			char** nombre = string_split(myfile->d_name, ".");
			dataArch->nombre = string_duplicate(nombre[0]);
			dataArch->tamanio = config_get_int_value(config_archivo, "TAMANIO");
			dataArch->tipo = config_get_string_value(config_archivo, "TIPO");
			char** padre = string_split(pathArchivos, "/");
			int counter =0; while(padre[counter] != NULL) counter++;
			for (int j=0; j < MAX_DIRECTORIOS; j++){
				if (directorios[j].index == atoi(padre[counter-1])){
					dataArch->padre = directorios[j].index;
					break;
				}
			}
			dataArch->disponible = true;
			dataArch->bloques = list_create();
			int cant_bloques = (dataArch->tamanio/MB) + (dataArch->tamanio % MB != 0);
			for(int i = 0; i < cant_bloques; i++){
				t_block* dataBlock = alloc(sizeof(t_block));
				dataBlock->original = config_get_array_value(config_archivo,
						string_from_format("BLOQUE%sCOPIA%s", string_itoa(i), string_itoa(0)));
				dataBlock->copia = config_get_array_value(config_archivo,
						string_from_format("BLOQUE%sCOPIA%s", string_itoa(i), string_itoa(1)));
				dataBlock->bytes = config_get_int_value(config_archivo,
						string_from_format("BLOQUE%sBYTES", string_itoa(i)));
				list_add(dataArch->bloques, dataBlock);
			}
			config_destroy(config_archivo);
			list_add(archivos, dataArch);
		}
	}
	closedir(mydir);

}

void ingresarDirectorios(char* path) {
	DIR* mydir;
	struct dirent* myfile;
	struct stat mystat;
	mydir = opendir(path);
	if(mydir != NULL){
		while ((myfile = readdir(mydir)) != NULL) {
			stat(myfile->d_name, &mystat);
			if (!string_equals_ignore_case(myfile->d_name, "..")
					&& !string_equals_ignore_case(myfile->d_name, ".")) {
				recuperarArchivos(string_from_format("%s%s", path, myfile->d_name));
			}
		}
	}
	closedir(mydir);
}

void restablecerEstado() {

	configYAMA->restore = false;
	t_config* restore_config = config_create(string_from_format("%s%s",configYAMA->ruta_metadata,"/nodos.bin"));
	fileDirectorios = fopen(string_from_format("%s%s", configYAMA->ruta_metadata,"/directorios.dat"), "wb+");
	if (!fileDirectorios) log_error(log_fs, "El archivo no se pudo abrir");

	int size_bytes;
	fseek(fileDirectorios, 0, SEEK_END);
	size_bytes = ftell(fileDirectorios);
	rewind(fileDirectorios);

	if (size_bytes== 0 || restore_config == NULL || !config_has_property(restore_config, "TAMANIO")){
		format_fs(configYAMA,directorios);
	}else{
		restablecerNodos(restore_config);
		for (int i = 0; i < MAX_DIRECTORIOS; i++) {
			fread(&directorios[i], sizeof(t_directory), 1, fileDirectorios);
		}

		ingresarDirectorios(string_from_format("%s%s", configYAMA->ruta_metadata, "/archivos/"));
	}


}
