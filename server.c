#include <netdb.h>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "messages.h"

int sockfd;
int newSockFd;

void terminationHandler(int sig) {
    close(sockfd);
    close(newSockFd);
    exit(0);
}

// function to read from client
// param is the reference to the new socket fd --> &newSockFd
void* reader(void *param){

    newSockFd = *(int*)(param);// retrieves the value of newSockFd from its address
    free(param);

    //////// REVERSE HASH FUNCTION
    // Read in request through new socket
    uint8_t buffer[PACKET_REQUEST_SIZE];
    read(newSockFd, buffer, PACKET_REQUEST_SIZE);

    // Declare request components
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t p;

    // Extract components from request
    memcpy(hash, buffer + PACKET_REQUEST_HASH_OFFSET, 32);
    memcpy(&start, buffer + PACKET_REQUEST_START_OFFSET, 8);
    memcpy(&end, buffer + PACKET_REQUEST_END_OFFSET, 8);
    memcpy(&p, buffer + PACKET_REQUEST_PRIO_OFFSET, 1);

    // Convert byte order as needed
    start = htobe64(start);
    end = htobe64(end);

    // Search for key in given range corresponding to given hash
    uint8_t calculatedHash[32];
    uint64_t key;
    for (key = start; key < end; key++) {
        SHA256((uint8_t *)&key, 8, calculatedHash);
        if (memcmp(hash, calculatedHash, 32) == 0)
            break;
    }

    // Send resulting key back to client
    key = be64toh(key);
    write(newSockFd, &key, 8);

    close(newSockFd);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    // Set up signal for graceful termination
    signal(SIGINT, terminationHandler);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);           // Create socket --> holds the return of the socket function

    // Set the port as available in case it is not available, and check for error
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }

    // Initialize socket structure
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Bind to host address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR when accepting");
        exit(1);
    }

    int NUM_CONNECTIONS;                // number of connections
    NUM_CONNECTIONS = 100;              // set to 50 for testing purposes

    listen(sockfd, NUM_CONNECTIONS);        // Listen for client --> waits for client to make connection with server

    // Prepare client address and size
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    // Fork into 4 processes, one for each core
    fork();
    fork();

    // Begin accepting client connections as concurrent threads
    while (1) {
        // Accept connection ( will take the first in the queue)
        newSockFd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        // check for error
        if (newSockFd < 0) {
            perror("ERROR on accept");
            exit(1); // exit when the newSockFd < 0 as no more requests
        }
        int *newSockFdPtr = malloc(sizeof(int));
        memcpy(newSockFdPtr, &newSockFd, sizeof(int));
        pthread_t tid;
        pthread_create(&tid, NULL, reader, newSockFdPtr);
    }
}






