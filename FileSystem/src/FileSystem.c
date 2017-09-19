#include "FileSystem.h"
#include "server.h"

int main(int arg, char** argv) {

	set_current_process(FS);
	log_fs = log_create("../Log","FileSystem",false,LOG_LEVEL_INFO);
	config = get_config("../Configuracion");

	if (argv[1] != NULL && streq(argv[1], "--clean")){
		log_info(log_fs,"Iniciar ignorando/eliminando estado anterior");
		format_fs(config,directorios);
	}
	else{
		restablecerEstado();

	}

	title("File System");

	server_start(config);
	inicializarConsola();

	fclose(fileDirectorios);
	fclose(fileNodos);
	fclose(fileBitmap);
	return EXIT_SUCCESS;
}


t_fileSystem *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_fileSystem *config = malloc(sizeof(t_fileSystem));

	config->puerto_fs = config_get_string_value(c, "PUERTO_FS");
	config->ruta_metadata = config_get_string_value(c, "RUTA_METADATA");
	config->nodosEstable = 0;
	config->stable = false;

	title("Configuracion");
	printf("PUERTO FS: %s\n", config->puerto_fs);
	printf("RUTA METADATA: %s\n", config->ruta_metadata);

	return config;
}

int importarArchivo(char* location, char* destino){

	FILE* file;
	if (!(file = fopen(location, "r"))){
		log_error(log_fs, "El archivo no existe o no se pudo abrir");
	}
	else{
		//validar si el destino es valido
		int size_bytes;
		fseek(file,0,SEEK_END);
		size_bytes = ftell(file);
		rewind(file);

		int cant_bloques = (size_bytes/MB) + (size_bytes % MB != 0);

		char* map;
		if((map = mmap(NULL, size_bytes, PROT_READ, MAP_SHARED, fileno(file),0)) == MAP_FAILED){
			log_error(log_fs,"Error al mappear archivo\n");
		}
		for(int i = 0; i < cant_bloques; i++){
			enviarADataNode(map, i, size_bytes);
		}
	}
	fclose(file);
	return 0;
}

void enviarADataNode(char* map, int bloque, int size_bytes){

	unsigned char buffer[BUFFER_CAPACITY];
	header_t header_setBlock = protocol_header(OP_SET_BLOQUE);
	header_setBlock.msgsize = serial_pack(buffer,"hs",bloque,map);
	packet_t packet_setBlock = protocol_packet(header_setBlock, buffer);

	bool getClient(void *nbr) {
		client_t *unCliente = nbr;
		return (DATANODE == unCliente->type);
	}
	client_t *cliente = list_find(server.clients, getClient);

	protocol_packet_send(packet_setBlock,cliente->socket);

}

void format_fs(t_fileSystem *config,t_directory directorios[MAX_DIRECTORIOS]){

	system(string_from_format("exec rm -r %s%s",config->ruta_metadata,"/*"));
    mkdir(string_from_format("%s%s",config->ruta_metadata,"/archivos/"), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(string_from_format("%s%s",config->ruta_metadata,"/bitmaps/"), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    config->restore = true;
	config->nodosEstable = 2;
	directorios[0].index = 0;
	directorios[0].padre = -1;
	strcpy(directorios[0].nombre, "/");
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
	t_nodos_table* tablaNodos = malloc(sizeof(t_nodos_table));
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
				string_from_format("%s%s%s%s", config->ruta_metadata,
						"/bitmaps/", tablaNodos->nombre_nodos[i], ".dat"),
				"rb+"))) {
			log_error(log_fs, "El archivo no se pudo abrir");
		} else {
			int size_bytes;
			fseek(fileBitmap, 0, SEEK_END);
			size_bytes = ftell(fileBitmap);
			rewind(fileBitmap);

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

void restablecerEstado() {

	config->restore = false;
	t_config* restore_config = config_create(string_from_format("%s%s",config->ruta_metadata,"/nodos.bin"));
	if (restore_config == NULL){
		log_error(log_fs, "El archivo no se pudo abrir");
	}else{
		restablecerNodos(restore_config);
	}

	if (!(fileDirectorios = fopen(string_from_format("%s%s", config->ruta_metadata,"/directorios.dat"), "wb+"))) {
		log_error(log_fs, "El archivo no se pudo abrir");
	} else {
		for (int i = 0; i < MAX_DIRECTORIOS; i++) {
			fread(&directorios[i], sizeof(t_directory), 1, fileDirectorios);
		}
	}

}

