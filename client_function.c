#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    /********
     complete by Siyuan Sun
     ********/
   printf("which protocol do you want to use? TCP or UDP?"); //prompt user to decide TCP or UDP protocol
   char a[5];                                       //get answer
   scanf("%s", a);
    //if user choose UDP protocol
   if (strcmp(a,"UDP") == 0)
   {
       int sock, n;                                 //sock and status
       unsigned int length;                         //length of address
       struct sockaddr_in server, from;             //adress of server
       struct hostent *hp;                          //host port number
       char buffer[256];                            //input buffer
       
       if (argc != 3) { printf("Usage: server port\n");     //check whether user input a valid input
           exit(1);
       }
       sock= socket(AF_INET, SOCK_DGRAM, 0);                //create socket
       if (sock < 0) error("socket");                       //if socket is not created, print error message
                                                            //then quit
       
       server.sin_family = AF_INET;                         //internet
       hp = gethostbyname(argv[1]);                         //host name
       if (hp==0) error("Unknown host");                    //if host doesn't exist or wrong, print out
                                                            //error message
       
       bcopy((char *)hp->h_addr,                            //copy IP address of server to client
             (char *)&server.sin_addr,
             hp->h_length);
       server.sin_port = htons(atoi(argv[2]));              //port number
       length=sizeof(struct sockaddr_in);                   //size of struct socket address
       printf("Please enter the message: ");
       bzero(buffer,256);                                   //intiatilized the buffer
       scanf("%s",buffer);                                  //get the input into buffer
       //fgets(buffer,255,stdin);
       n=sendto(sock,buffer,                                //send buffer to server side
                strlen(buffer),0,(const struct sockaddr *)&server,length);
       if (n < 0) error("Sendto");                          //if send is not sucessful, print error message
       n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);   //read message from server
       if (n < 0) error("recvfrom");                        //pirnt error message if receive not successful
       write(1,"Got an ack: ",12);                          //show the user acknowledge from server
       write(1,buffer,n);
       close(sock);
   }
    /********
     complete by Qi Gao
     ********/
    //if the user choose TCP protocol
   if (strcmp(a,"TCP") == 0) 
   {
       //error check
   if (argc < 3) {
         fprintf(stderr,"usage %s hostname port\n", argv[0]);
         exit(0);
   }
       
    int sockfd, portno, n;                              //sock and status adn prot number
    struct sockaddr_in serv_addr;                       //adress of server
    struct hostent *server;                             //IP address of server
    char buffer[256];                                   //input buffer
    portno = atoi(argv[2]);//get port number from argument
    sockfd = socket(AF_INET, SOCK_STREAM, 0);           //get socket
       
    //if socket is not created, print error message, then quit
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);                    //get host name
       //if host name is null, print error message
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);}
    bzero((char *) &serv_addr, sizeof(serv_addr));      //initialized server address
    serv_addr.sin_family = AF_INET;                     //internet
    bcopy((char *)server->h_addr,                       //get server address
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);                 //get port number
       //try to connect with server side, if not sucessful, print out error message
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
       
    printf("Please enter the message: ");
    bzero(buffer,256);                                  //initialized buffer
    scanf("%s",buffer);                                 //get input and put into buffer
    n = write(sockfd,buffer,strlen(buffer));            //write a message and send to server and get number
                                                        //the size of buffer
       //if the message is not sent to server, print out the error message
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);                                  //read message buffer from server
    n = read(sockfd,buffer,255);                        //read message from server and get size of buffer
       //if the message is not received from server, print out the error message
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);                             //print out the message from server
    close(sockfd);                                     //close the socket
    }
    return 0;
}
