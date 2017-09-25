#include "FileSystem.h"
#include "server.h"
#include "inicializacion.h"

int main(int arg, char** argv) {

	set_current_process(FS);
	log_fs = log_create("../Log","FileSystem",false,LOG_LEVEL_INFO);
	config = get_config("../Configuracion");
	archivos = list_create();

	if (argv[1] != NULL && streq(argv[1], "--clean")){
		log_info(log_fs,"Iniciar ignorando/eliminando estado anterior");
		format_fs(config,directorios);
	}
	else{
		restablecerEstado();
		for(int i = 0; i < list_size(archivos); i++){
			t_arch* archivo = list_get(archivos, i);
			printf("%s\n", archivo->nombre);

		}
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
		int tam = 0;
		char* map;
		if((map = mmap(NULL, size_bytes, PROT_READ, MAP_SHARED, fileno(file),0)) == MAP_FAILED){
			log_error(log_fs,"Error al mappear archivo\n");
		}
		for(int i = 0; i < cant_bloques; i++){
			if(size_bytes > MB){
				enviarADataNode(map, i, tam, MB);
				tam += MB;
				size_bytes -= MB;
			}else{
				size_bytes++;
				enviarADataNode(map, i, tam, size_bytes);
			}
		}
	}
	fclose(file);
	return 0;
}

void enviarADataNode(char* map, int bloque, int tam, int size_bytes){

	unsigned char buffer[BUFFER_CAPACITY];
	unsigned char buff[MB];
	memcpy(buff, map+tam, size_bytes-1);
	if(size_bytes != MB) printf("%s\n", buff);
	header_t header_setBlock = protocol_header(OP_SET_BLOQUE);
	header_setBlock.msgsize = serial_pack(buffer,"hs",bloque,buff);
	packet_t packet_setBlock = protocol_packet(header_setBlock, buffer);

	bool getClient(void *nbr) {
		client_t *unCliente = nbr;
		return (DATANODE == unCliente->type);
	}
	client_t *cliente = list_find(server.clients, getClient);

	protocol_packet_send(packet_setBlock,cliente->socket);
	memset(buff, 0, MB);
}


