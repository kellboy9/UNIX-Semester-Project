#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		printf("Error creating socket.\n");
	}
	struct sockaddr_in server_addr;  
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(3000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Error binding socket.\n");
	}

	printf("Run the client.\n");

	//listen for clients
	listen(sock, 10);
	struct sockaddr_storage client_addr;
	//block until connected
	socklen_t client_addr_size = sizeof(client_addr);
	int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_size);
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
	}

	close(client_sock);
	close(sock);
	return 0;
}
