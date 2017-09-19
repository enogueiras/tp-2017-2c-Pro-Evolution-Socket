#include "FileSystem.h"
#include "server.h"

int main(int arg, char** argv) {

	set_current_process(FS);
	log_fs = log_create("../Log","FileSystem",false,LOG_LEVEL_INFO);
	config = get_config("../Configuracion");

	if (argv[1] != NULL && streq(argv[1], "--clean")){
		log_info(log_fs,"Iniciar ignorando/eliminando estado anterior");
	}
	else{
		t_config* restore_config = config_create("metada/nodos.dat");
		if (restore_config == NULL) puts(string_from_format("BLOQUE%s%s%s",string_itoa(0),"COPIA",string_itoa(0)));
	}

	title("File System");

	server_start(config);
	inicializarConsola();
	return EXIT_SUCCESS;
}


t_fileSystem *get_config(const char *path) {
	t_config* c = config_create((char *) path);
	t_fileSystem *config = malloc(sizeof(t_fileSystem));

	config->puerto_fs = config_get_string_value(c, "PUERTO_FS");
	config->nodosEstable = 0;
	config->stable = false;
	title("Configuracion");
	printf("PUERTO FS: %s\n", config->puerto_fs);

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

		int cant_bloques = 1;
		char* map;
		if((map = mmap(NULL, size_bytes, PROT_READ, MAP_SHARED, fileno(file),0)) == MAP_FAILED){
			printf("Error al mappear archivo\n");
		}
		int i;
		for(i = 0; i < cant_bloques; i++){
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
