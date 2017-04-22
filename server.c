#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "server_functions.h"

//AUTH: Everyone pretty much

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Error: Need exactly 1 argument (port #)\n");	
		exit(1);
	}

	struct serv *the_server = init_serv(atoi(argv[1]));
	if (!the_server) { // init_serv returns NULL on failure
		error("Could not init server");
	}

	int pid = fork();
	if (pid == -1) {
		error("Could not create child process");
	}

	if (pid == 0) {
		if (tcp_proc(the_server) == 1) {
			error("Error in handling TCP\n");
		}
	}

	else {
		if (udp_proc(the_server) == 1) { // Actually, udp_proc starts an infinite loop, so it should never exit.
			error("Error in handling UDP\n");
		}		
	}

	close_serv(the_server);
	return 0;
}
