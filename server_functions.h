#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

//AUTH: Keller Hood
struct serv {
	int tcp_fd;
	int udp_fd;
};
struct serv *init_serv(int port); // NOTE: init_serv accepts only 1 argument now -- Enoch
void close_serv(struct serv *server);

//AUTH: Jorge Macias
void tcp_comm(int, struct serv *server, struct sockaddr_in cli_addr);
int tcp_proc(struct serv *server);
int udp_proc(struct serv *server);

void error(const char *msg);
void set_log_port(int log_port);
void set_log_ip(const char *log_ip);

#endif
