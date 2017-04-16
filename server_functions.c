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

struct serv *init_serv(const char ip[14], int port) {
	struct serv *new_serv = malloc(sizeof(struct serv));

	//create + bind + listen tcp socket
	int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_sock < 0) {
		printf("Error creating socket.\n");
	}
	struct sockaddr_in server_addr;  
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	if(bind(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Error binding socket.\n");
	}
	listen(tcp_sock, 5);

	new_serv->tcp_fd = tcp_sock;


	//int udp_sock = ...
	//TODO: UDP
	//new_serv->udp_fd = udp_sock;
	
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

int tcp_proc(int argc, char *argv[]) //tcp socket process: Jorge Macias
{
	int sockfd, newsockfd, portno, procid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	//implement while loop: Jorge Macias
	while (1)
	{
		newsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		//implement fork call: Jorge Macias
		procid = fork();
		if (procid < 0) 
			error("ERROR on fork");
		if (procid == 0)
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
