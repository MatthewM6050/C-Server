/* Program: server.c
* A simple TCP server using sockets.
* Server is executed before Client.
* Port number is to be passed as an argument.
*
* To test: Open a terminal window.
* At the prompt ($ is my prompt symbol) you may
* type the following as a test:
*
* $ ./server 54554
* Run client by providing host and port
*
*
*/

#include <ctype.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void toUpperHelper(char *string){
	int i = 0;
	char ch;
		
	while(string[i]){
		ch = string[i];
		string[i] = toupper(ch);
		i++;
	}
}

int main(int argc, char *argv[])
{
	FILE *fptr;
	FILE *rfptr;
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    fprintf(stdout, "Run client by providing host and port\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    if (sockfd < 0)
        error("ERROR opening socket");
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
 
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen); 
    if (newsockfd < 0)
        error("ERROR on accept");

	// Reads from socket
    	bzero(buffer,256);
    	n = read(newsockfd,buffer,255);
    
    // Grabs the command from user
    char* token = strtok(buffer, " \n");
    
    // Compares commands
    if(strcmp(token, "toUpper") == 0){// Compares to see if toUpper command is called
    	char temp[100];
        token = strtok(NULL, " \n");// Grabs the fileName
        
        fptr = fopen(token, "r");// Opens file being read from
        if(fptr == NULL){// Checks file exists
            fprintf(stderr,"ERROR in reading file.\n");
            exit(0);
        }
        
        rfptr = fopen("recv.txt", "w");// Opens file being written to
        if(rfptr == NULL){// Checks file exists
            fprintf(stderr,"ERROR in writing file.\n");
            exit(0);
        }
        
	while(fgets(temp, 100, fptr) != NULL){ // Reads every line from the file
		toUpperHelper(temp);// Helper function to uppercase temporary string
		fprintf(rfptr, "%s", temp);// Prints temporary string into new file
	}
	if(feof(fptr)){ // Checks if file is at EOF
		fclose(fptr); // Closes File that is read from
        	fclose(rfptr);// Closes File that is written to
        	fprintf(stderr,"File Data Processed.\n");
        }
        bzero(temp, 100);
    }
    else if(strcmp(token, "count") == 0){
    	// Variables
    	int count = 0;
    	char c;
	char* char_filter = strtok(NULL, " \n");
	char filter = *char_filter;
	printf("%s", char_filter);
	char* filename = strtok(NULL, " \n");
	
	fptr = fopen(filename, "r");
	if(fptr == NULL){// Checks file exists
            fprintf(stderr,"ERROR in reading file.\n");
            exit(0);
        }
        
	rfptr = fopen("count.txt", "w");
	if(rfptr == NULL){// Checks file exists
            fprintf(stderr,"ERROR in writing file.\n");
            exit(0);
        }
        
        for( c = getc(fptr); c != EOF; c = getc(fptr)){
        	if(c == filter)
        		count++;
        }
        
        if(feof(fptr)){
        	fclose(fptr);
        	fprintf(rfptr, "%d", count);
        	fclose(rfptr);
        	fprintf(stderr,"File Data Processed.\n");
        }
    }
 
    if (n < 0)
        error("ERROR reading from socket");
 
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    
    if (n < 0)
        error("ERROR writing to socket");
    
    close(newsockfd);
    close(sockfd);
    return 0;
}
