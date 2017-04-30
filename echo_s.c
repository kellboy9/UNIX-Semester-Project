#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#include "server_functions.h"

char* log_s_ip;
char* log_s_port;

// Signal handler function to respond to Ctrl+C interrupt signal -- Enoch Ng
void handler(int s) { 
	printf("Received interrupt signal, shutting down");
	
	// Send a message to the log server now
	
	int sock, n;
	unsigned int length;
	struct sockaddr_in server, from;
	struct hostent *hp;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("Error creating socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	hp = gethostbyname(log_s_ip);
	if (hp == 0) {
		printf("Error: unknown host");
		exit(1);
	}

	bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(log_s_port));
	length = sizeof(struct sockaddr_in);
	char* msg = "echo_s is stopping";
	n = sendto(sock, msg, strlen(msg), 0, (const struct sockaddr*) &server, length);
	if (n < 0) {
		printf("Error sending message");
		exit(1);
	}
	printf("Sent shutdown message to log server\n");
	close(sock);
	exit(0);
}

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
	close_serv(the_server); 
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
	int i;
	for (i = 0; i < argc - 1; i++) {
		ports[i] = atoi(argv[i + 1]);
	}

	// Need to assign the log server and log port to those variables here

	log_s_ip = "127.0.0.1";
	log_s_port = "4444"; // Make these variables depend on the arguments later
	
	// --------------------------------------
	// Signal handler
	struct sigaction sigIntHandler;
	memset(&sigIntHandler, 0, sizeof(sigIntHandler));
	sigIntHandler.sa_handler = handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	//sigset(SIGINT, handler);
	// --------------------------------------
	
	// Accept on multiple ports functionality - Enoch Ng
	// For the init_serv call, we'll fork the program 0-2 times (depending on the amount of ports), and call init_serv in each process
	// If the port limit were much higher, checking for every case with if-statements would be infeasible, but as it is, in the interest of time, I'm okay with just doing things the "brute force" way ...

	if (argc > 2) {
		int pid = fork();
		
		if (pid == 0) {
			run_serv(ports[0]);
		}

		else {
			if (argc > 3) {	
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
