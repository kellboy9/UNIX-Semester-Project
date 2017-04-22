#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "server_functions.h"

int run_serv(int port) { // I moved most of the actual code in the main() function to this function -- Enoch
	struct serv *the_server = init_serv(port);
	if (!the_server) {
		error("Could not init server");
	}

	int pid = fork();
	if (pid == -1) {
		error("Could not create child process");
	}
	if (pid == 0) {
		if (tcp_proc(the_server) == 1) {
			error("Error in handling TCP");
		}
	}
	else {
		if (udp_proc(the_server) == 1) {
			error("Error in handling UDP");
		}
	}
	close_serv(the_server); // Will this ever be called?? tcp_proc and udp_proc loop forever unless they encounter an error...
	return 0;
}

//AUTH: Everyone pretty much

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Error: Need at least 1 port!\n");
		exit(1);
	}

	if (argc > 4) {
		printf("Error: No more than 3 ports allowed.\n");
		exit(1);
	}

	// Prepare arguments to be passed to init_serv -- Enoch Ng
	int ports[3];	
	for (int i = 0; i < argc - 1; i++) {
		ports[i] = atoi(argv[i + 1]);
	}

	// For the init_serv call, we'll fork the program 0-2 times (depending on the amount of ports), and call init_serv in each process
	if (run_serv(port[0]) != 0) {
		error("Error in running the server");
	}	
	return 0;
}
