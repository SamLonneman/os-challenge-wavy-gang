// NOTE: We used some socket logic from https://www.tutorialspoint.com/unix_sockets/index.htm

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


// Takes in pointer to argument on the heap: 49B client request plus 4B (int) newsockfd
void* reverseHash(void *arg) {

    // Declare components of argument
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t p;
    int newsockfd;

    // Extract components from argument
    memcpy(hash, arg + PACKET_REQUEST_HASH_OFFSET, 32);
    memcpy(&start, arg + PACKET_REQUEST_START_OFFSET, 8);
    memcpy(&end, arg + PACKET_REQUEST_END_OFFSET, 8);
    memcpy(&p, arg + PACKET_REQUEST_PRIO_OFFSET, 1);
    memcpy(&newsockfd, arg + PACKET_REQUEST_SIZE, sizeof(int));

    // Deallocate arg memory
    free(arg);

    // Search for key in given range corresponding to given hash
    uint8_t calculatedHash[32];
    uint64_t key;
    for (uint64_t i = htobe64(start); i < htobe64(end); i++) {
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, &i, sizeof(uint64_t));
        SHA256_Final(calculatedHash, &sha256);
        if (memcmp(hash, calculatedHash, 32) == 0) {
            key = i;
            break;
        }
    }

    // Send resulting key back to client
    key = be64toh(key);
    write(newsockfd, &key, 8);

    // Close socket and exit
    close(newsockfd);
    pthread_exit(NULL);
}

// Start server
int main(int argc, char *argv[]) {

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set the port as available in case it is not available, and check for error
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }

    // Initialize socket structure
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Bind to host address, and check for error
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Listen for client
    listen(sockfd, 100);

    // Prepare client address and size
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    // Accept client connections and assign each request to a thread
    while (1) {

        // Accept connection, and check for error
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // Dynamically allocate memory for thread argument with room for request and newsockfd
        void *arg= malloc(PACKET_REQUEST_SIZE + sizeof(int));

        // Read in request through new socket into thread argument
        read(newsockfd, arg, PACKET_REQUEST_SIZE);

        // Copy newsockfd into thread argument
        memcpy(arg + PACKET_REQUEST_SIZE, &newsockfd, sizeof(int));

        // Create thread to calculate and return response to client
        pthread_t tid;
        pthread_create(&tid, NULL, reverseHash, arg);
    }
}