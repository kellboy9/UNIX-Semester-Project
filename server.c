#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "server_functions.h"


//AUTH: Everyone pretty much

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Error: Need exactly 2 arguments (IP and port)\n");	
		exit(1);
	}

	struct serv *the_server = init_serv(argv[1], atoi(argv[2]));
	if (!the_server) { // init_serv returns NULL on failure
		printf("Could not init server");
		exit(1);
	}

	int pid = fork();
	if (pid == -1) {
		printf("Could not create child process");
		exit(1);
	}

	if (pid == 0) {
		if (server_functions::tcp_proc(the_server) == 1) {
			printf("Error in handling TCP\n");
			exit(1);
		}
	}

	else {
		if (server_functions::udp_proc(the_server) == 1) { // Actually, udp_proc starts an infinite loop, so it should never exit.
			printf("Error in handling UDP\n");
			exit(1);
		}		
	}

	// We will probably not need this code once we implement the stuff in the above if-statements
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
	/*struct sockaddr_storage client_addr;
	//block until connected
	socklen_t client_addr_size = sizeof(client_addr);
	int client_sock = accept(the_server->tcp_fd, (struct sockaddr*)&client_addr, &client_addr_size);
	if(client_sock < 0) {
		printf("Error accepting socket.\n");
	}
	printf("Got connected.\n");
	printf("Type in something and hit ENTER to send to the client.\n");
	write(client_sock, "Here!!\n", 16);

	char buf[16] = "";
	while(strcmp(buf, "exit\n") != 0) {
		fgets(buf, 16, stdin);
		write(client_sock, buf, 16);
	}*/

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	close(client_sock);

	close_serv(the_server);
	return 0;
}
