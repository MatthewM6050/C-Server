/*
* Simple client to work with server.c program.
* Host name and port used by server are to be
* passed as arguments.
*
* To test: Open a terminal window.
* At prompt ($ is my prompt symbol) you may
* type the following as a test:
*
* $./client 127.0.0.1 54554
* Please enter the message: Programming with sockets is fun!
* I got your message
* $
*
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    // Connecting to Server through IP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)// Error Handling
        error("ERROR opening socket");
    
    // Connecting to Server through hostName
    server = gethostbyname(argv[1]);
    if (server == NULL) {// Error handling
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
    (char *)&serv_addr.sin_addr.s_addr,
    server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Handling Commands
    printf("Please enter the message: ");
    bzero(buffer,256);
    // Grabs command from user
    fgets(buffer,255,stdin);
    //Sends command from client to Server
    send(sockfd, buffer, 255, 0);
    fprintf(stderr,"File data sent successfully.\n");
    
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
    error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
