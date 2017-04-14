#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void domorethanstuff(int);				#domorethanstuff function prototype *Jorge Macias

int main(int argc, char *argv[])
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
	 while (1)						#implement while loop *Jorge Macias
	 {
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
	 procid = fork();						#implement fork call *Jorge Macias
     if (procid < 0) 
		 error("ERROR on fork");
     if (procid == 0)
     	{
		close (sockfd);
		domorethanstuff(newsockfd);
		exit(0);
	 }
     else close(newsockfd);
     }									#end of while loop *Jorge Macias
	close(sockfd);
     return 0; 
}

void domorethanstuff(int sock)       #create domorethanstuff function to handle seperate instances of client communicationi
{
	int a;
	char buff;
	
	bzero(buff, 256);
	a = read(sock, buff, 255);
	if (a < 0) error ("ERROR reading from socket");
	prinf("Here is a message: %s\n", buff);
	a = write(sock, "Got some message", 18);
	if (a < 0) error("ERROR writing from socket");
}
