#include "server_functions.h"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}
//function prototype: Jorge Macias
void tcp_comm(int);

//AUTH: Keller Hood
//ARGS: ip is ip address in xxx.xxx.xxx.xxx format, 
//	port is port number, 
//RETURN: returns a struct containing two initialized sockets, one TCP and one UDP 

// Note: init_serv now returns NULL on error -- Enoch

struct serv *init_serv(const char ip[14], int port) {
	struct serv *new_serv = malloc(sizeof(struct serv));

	//create + bind + listen tcp socket
	int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_sock < 0) {
		printf("Error creating socket.\n");
		return NULL;
	}
	struct sockaddr_in server_addr;  
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	if(bind(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Error binding socket for TCP.\n");
		return NULL;
	}
	listen(tcp_sock, 5);

	new_serv->tcp_fd = tcp_sock;

	// UDP server init, AUTH: Enoch Ng
	int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock < 0) {
		printf("Error creating socket.\n");
		return NULL;
	}

	if (bind(udp_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
		printf("Error binding socket for UDP.\n");
		return NULL;
	}

	new_serv->udp_fd = udp_sock;

	return new_serv;
}

//AUTH: Keller Hood
//ARGS: a server struct containing tcp and udp fds
//frees server's memory and closes the sockets
void close_serv(struct serv *server) {
	close(server->tcp_fd);
	close(server->udp_fd);
	free(server);
}

int tcp_proc(struct serv *server) //tcp socket process: Jorge Macias
{
	int sockfd, newsockfd, portno, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	clilen = sizeof(cli_addr);
	//implement while loop: Jorge Macias
	while (1)
	{
		newsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		//zombie process signal handler: Jorge Macias
		signal(SIGCHLD,SIG_IGN);
		//implement fork call: Jorge Macias
		pid = fork();
		if (pid < 0) 
			error("ERROR on fork");
		if (pid == 0)
		{
			close (sockfd);
			tcp_comm(newsockfd); //call tcp_comm function: Jorge Macias
			exit(0);
		}
		else close(newsockfd);
	} //end of while loop: Jorge Macias
	close(sockfd);
	return 0; 
}

// Handle UDP
// Returns 1 on failure
// AUTH: Enoch Ng
// Child processes are not needed here because UDP is not connection-based, so the server is able to respond to multiple clients without needing multiple processes
int udp_proc(struct serv *server) {
	char buf[1024];
	socklen_t clilen;
	struct sockaddr_in cli_addr;
	clilen = sizeof(cli_addr);

	while (1) {
		int n = recvfrom(server->udp_fd, buf, 1024, 0, (struct sockaddr*) &cli_addr, &clilen); 
		if (n < 0) {
			printf("Error reciving UDP message\n");
			return 1;
		}

		write(1, "Received a datagram: ", 21);
		write(1, buf, n);

		n = sendto(server->udp_fd, "Got your message\n", 17, 0, (struct sockaddr*) &cli_addr, clilen);
		
		if (n < 0) {
			printf("Error in sending reply to UDP message\n");
			return 1;
		}
	}

	return 0; // This should never happen ...
}

//function to handle seperate instances of client communication: Jorge Macias
void tcp_comm(int sock)
{
	int a;
	char buff[256];

	bzero(buff, 256);
	a = read(sock, buff, 256);
	if (a < 0) error ("ERROR reading from socket");
	printf("Here is a message from somewhere: %s\n", buff);
	a = write(sock, "Got some message from someplace", 18);
	if (a < 0) error("ERROR writing from socket");
}
