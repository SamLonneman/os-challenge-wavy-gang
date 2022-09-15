#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {

    /* First call to socket() function */
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    /* Initialize socket structure */
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Bind to host address
    bind(socketDescriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    // Listen for client
    listen(socketDescriptor, 5);

    // Accept connection from client
    struct sockaddr_in cli_addr;
    int clientLength = sizeof(cli_addr);
    int newsockfd = accept(socketDescriptor, (struct sockaddr *)&cli_addr, &clientLength);

    // Start communicating
    char buffer[49];
    bzero(buffer,49);
    read( newsockfd,buffer,49 );

    printf("Here is the message: %s\n",buffer);
    write(newsockfd,"I got your message",18);

    return 0;
}