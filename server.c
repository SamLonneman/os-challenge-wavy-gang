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
int requestCounter;

// each priority level has a row which stores the requests to be run
int priorityArray[16][300] = {0};
uint64_t startArray[16][300] = {0};
uint64_t endArray[16][300] = {0};
uint8_t * hashArray[16][300] = {0};





void terminationHandler(int sig) {
    close(sockfd);
    close(newSockFd);
    exit(0);
}

// function to read from client
// param is the reference to the new socket fd --> &newSockFd






int main(int argc, char *argv[]) {
// first element in each row is a count of the nest place to be filled in the array
    priorityArray[0][0] = 1;
    priorityArray[1][0] = 1;
    priorityArray[2][0] = 1;
    priorityArray[3][0] = 1;
    priorityArray[4][0] = 1;
    priorityArray[5][0] = 1;
    priorityArray[6][0] = 1;
    priorityArray[7][0] = 1;
    priorityArray[8][0] = 1;
    priorityArray[9][0] = 1;
    priorityArray[10][0] = 1;
    priorityArray[11][0] = 1;
    priorityArray[12][0] = 1;
    priorityArray[13][0] = 1;
    priorityArray[14][0] = 1;
    priorityArray[15][0] = 1;




    // Set up signal for graceful termination
    signal(SIGINT, terminationHandler);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);           // Create socket --> holds the return of the socket function

    // Set the port as available in case it is not available, and check for error
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0) {
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

    // Accept client connections until there are none left
    while (1) {
        // Accept connection ( will take the first in the queue)
        newSockFd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        requestCounter++;

        // check for error
        if (newSockFd < 0) {
            perror("ERROR on accept");
            exit(1); // exit when the newSockFd < 0 as no more requests
        }

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

        int arraySpot = priorityArray[p - 1][0];            // find spot in array for this request
        priorityArray[p - 1][0] = priorityArray[p - 1][0] + 1; // increment count

        hashArray[p - 1][arraySpot] = hash;
        startArray[p - 1][arraySpot] = start;
        endArray[p - 1][arraySpot] = end;
        priorityArray[p - 1][arraySpot] = newSockFd;                   // indicate there is a request with != NULL
        // TODO: Change this to 250 for submission
        if (requestCounter == 25) {
            //goto priorityLoop;
            int p;

            p = 0;
            int i;
            i = 15;
            while (i > -1) {
                int j;
                j = priorityArray[i][0];
                while (j > 0 && priorityArray[i][j] !== 0) {

                    p++;
                    printf("%d\n",p);
                    // work on request in place priorityArray[i][priorityArray[i][0]-1]
                    // Convert byte order as needed
                    uint64_t start = htobe64(startArray[i][j]);
                    uint64_t end = htobe64(endArray[i][j]);
                    uint8_t *hash = hashArray[i][j];
                    newSockFd = priorityArray[i][j];

                    // Search for key in given range corresponding to given hash
                    uint8_t calculatedHash[32];
                    uint64_t key;

                    for (key = start; key < end; key++) {
                        SHA256((uint8_t * ) & key, 8, calculatedHash);
                        if (memcmp(hash, calculatedHash, 32) == 0)
                            break;
                    }

                    // Send resulting key back to client
                    key = be64toh(key);
                    write(newSockFd, &key, 8);
                    close(newSockFd);
                    j = j - 1;
                }
                i = i - 1;
            }
        }
        else{
            printf("IDK what happened")
        }
    }
    return 0;
}