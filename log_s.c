#include <sys/types.h> // include all the library
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
 /********
 complete by Siyuan Sun
 ********/

int main(int argc, char *argv[])
{
    int sock, length, n;//define the socket, the length of address and the status
    socklen_t fromlen; // define the length 
    struct sockaddr_in server;//define the structure of the address of log.s
    struct sockaddr_in from; // define the structure of the address of echo.s
    
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n"); // if there is a error tell the user that no port number provided
        exit(0);
    }
    
    sock=socket(AF_INET, SOCK_DGRAM, 0); // create a scoket
    if (sock < 0) error("Opening socket");// output the error message if creating a socket is failed
    length = sizeof(server);// get the size of log.s address
    bzero(&server,length);// initial the address to 0
    server.sin_family=AF_INET; // fill the structure with inforamtion of the log.s
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));
    if (bind(sock,(struct sockaddr *)&server,length)<0)//bind the socket to an address
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
    /********
     complete by Qi Gao
     ********/
    while (1) {
		char buf[1024];// define the buffer to store the message get from echo.s
		memset(buf, 0, sizeof(buf));// set the buffer to zero
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);// get the message from the echo.s
        if (n < 0) error("recvfrom");// print out error message if the message is not got correctly
        printf("Got message\n");
	
	// -------------------------------
	// Functionality to close the log server -- Enoch Ng (3rd deliverable, user 4)
	if (strcmp("echo_s is stopping", buf) == 0) {
		printf("Received shutdown signal");
		close(sock);
		return 0;
	}
	// -------------------------------
	
	FILE *fp; 
	if((fp=fopen("echo.log","at+"))==NULL){// open a file
	        printf("Cannot open file strike any key exit!");// if not successfully opened, print out an error message
	        exit(1);
			                }
	int numChar=sizeof(buf);
	fputs(buf,fp);// write the meassage get from echo.s to the file
	fclose(fp);
	if(strcmp(buf,"echo_s is stopping")==0){
		break;
    }
    return 0;
}
