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
		printf("There was a problem starting the server. Hint: Double-check to make sure that you don't have multiple of the same port in your arguments.\n");
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

	// Prepare arguments to be passed to init_serv -- Enoch Ng
	int ports[3];	
	int i;
	for (i = 0; i < argc - 1; i++) {
		ports[i] = atoi(argv[i + 1]);
		if(argc > i+2 && strcmp(argv[i+2], "-logip") == 0) {
			//we're done with ports if we hit the logip option
			//(manually i++ since breaking the loop won't execute the i++ above)
			i++;
			break;
		}
		if(i+1 > 3) {
			printf("No more than 3 ports allowed!\n");
			exit(1);
		}
	}
	int num_ports = i;
	printf("number of ports entered: %d\n", num_ports);

	char* log_argv=argv[i++];  //add some comment
	char* logip=argv[i];
	set_log_ip(logip);
	
	char *logport_arg = argv[++i];
	char *logport = argv[++i];
	if(strcmp(logport_arg, "-logport") == 0) {
		//unsafe atoi
		set_log_port(atoi(logport));
	}

	// Accept on multiple ports functionality - Enoch Ng
	// For the init_serv call, we'll fork the program 0-2 times (depending on the amount of ports), and call init_serv in each process
	// If the port limit were much higher, checking for every case with if-statements would be infeasible, but as it is, in the interest of time, I'm okay with just doing things the "brute force" way ...
	if (num_ports > 1) {
		int pid = fork();
		
		if (pid == 0) {
			run_serv(ports[0]);
		}

		else {
			if (num_ports > 2) {	
				int pid2 = fork();
				// I feel bad, but, not really
				if (pid2 == 0) {
					run_serv(ports[1]);
				}

				else {
					run_serv(ports[2]);
				}
			}

			else {
				// Only 2 ports
				run_serv(ports[1]);
			}
		}
	}

	else {
		// Only 1 port, hooray
		run_serv(ports[0]);
	}
	return 0;
}
