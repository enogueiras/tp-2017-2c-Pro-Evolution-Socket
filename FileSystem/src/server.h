#ifndef SERVER_H_
#define SERVER_H_

#include "FileSystem.h"

void srv_thread(char*);

void server_start(t_fileSystem*);

void server_end();

void cli_thread(client_t*);

#endif /* SERVER_H_ */
