#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <openssl/sha.h>

#include "messages.h"

int main(int argc, char *argv[]) {

    // Create Socket
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Setting the port available in case of ERROR
    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    // Initialize socket structure
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Bind to host address
    if (bind(socketDescriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
    }

    // Listen for client
    listen(socketDescriptor, 5);

    // Accept connection from client
    struct sockaddr_in cli_addr;
    int clientLength = sizeof(cli_addr);
    int newSocketDescriptor = accept(socketDescriptor, (struct sockaddr *)&cli_addr, &clientLength);

    // Check for accept error
    if (newSocketDescriptor < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    // Start communicating
    char buffer[PACKET_REQUEST_SIZE];
    bzero(buffer, PACKET_REQUEST_SIZE);
    read(newSocketDescriptor, buffer, PACKET_REQUEST_SIZE);

    // Prepare request components
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t p;

    // Extract components from request
    memcpy(hash, buffer + PACKET_REQUEST_HASH_OFFSET, 32);
    memcpy(&start, buffer + PACKET_REQUEST_START_OFFSET, 8);
    memcpy(&end, buffer + PACKET_REQUEST_END_OFFSET, 8);
    memcpy(&p, buffer + PACKET_REQUEST_PRIO_OFFSET, 1);

    // Convert byte order
    start = htobe64(start);
    end = htobe64(end);

    // Debugging print
    printf("Start: %llu\nEnd: %llu\n", start, end);

    // Reverse hash
    uint8_t calculatedHash[32];
    uint64_t key;
    for (uint64_t i = start; i < end; i++) {
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, &i, 8);
        SHA256_Final(calculatedHash, &sha256);
        if (memcmp(hash, calculatedHash, 32) == 0) {
            key = i;
            break;
        }
    }

    // Send resulting key back to client
    key = be64toh(key);
    write(newSocketDescriptor, &key, 8);

    return 0;
}