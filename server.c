#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include "messages.h"

int main(int argc, char *argv[]) {

    /* First call to socket() function */
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    /* Initialize socket structure */
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Bind to host address
    bind(socketDescriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Listen for client
    listen(socketDescriptor, 5);

    // Accept connection from client
    struct sockaddr_in cli_addr;
    int clientLength = sizeof(cli_addr);
    int newSocketDescriptor = accept(socketDescriptor, (struct sockaddr *)&cli_addr, &clientLength);

    // Start communicating
    char buffer[PACKET_REQUEST_SIZE];
    bzero(buffer, PACKET_REQUEST_SIZE);
    read(newSocketDescriptor, buffer, PACKET_REQUEST_SIZE);

    char hash[32];
    unsigned long long start;
    unsigned long long end;
    char p;

    memcpy(hash, buffer + PACKET_REQUEST_HASH_OFFSET, 32);
    memcpy(&start, buffer + PACKET_REQUEST_START_OFFSET, 8);
    memcpy(&end, buffer + PACKET_REQUEST_END_OFFSET, 8);
    memcpy(&p, buffer + PACKET_REQUEST_PRIO_OFFSET, 1);

    printf("Message:\n\thash:\n\tstart: %llu\n\tend: %llu\n\tpriority: %d\n", start, end, p);

    unsigned long long response = 0x0100000000000000;
    write(newSocketDescriptor, &response, 8);

    return 0;
}