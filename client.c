#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

//AUTH: Keller Hood

int main(int argc, char **argv) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		printf("Error creating socket.\n");
	}
	struct sockaddr_in server_addr;  
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(3000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Error on connecting.\n");
	}
	char buf[16] = "";
	while(strcmp(buf, "kill\n") != 0) {
		read(sock, buf, 16);
		printf("%s", buf);
	}
	close(sock);
	return 0;
}
