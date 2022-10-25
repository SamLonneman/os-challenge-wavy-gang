#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>

#include <openssl/sha.h>

#include <signal.h>
#include <unistd.h>

#include "messages.h"

#include <semaphore.h>  // included from https://www.geeksforgeeks.org/handling-multiple-clients-on-server-with-multithreading-using-socket-programming-in-c-cpp/

// NOTE: Some socket logic taken from https://www.tutorialspoint.com/unix_sockets/index.htm


// Socket variables are global so that they can be closed by handler.
int sockfd;               // holds the return of the socket function
int newsockfd;
int sem_post(sem_t *sem); // increments the semaphore currently being pointed to
int sem_wait(sem_t *sem); // decrements the semaphore currently being pointed to
sem_t x, y;
pthread_t tid;
pthread_t writerthreads[100];
pthread_t readerthreads[100];
int readercount = 0;

// CTRL+C interrupt handler for graceful termination
void terminationHandler(int sig) {
    close(sockfd);
    close(newsockfd);
    exit(0);
}



// TODO: IMPLEMENT REVERSE HASHING WITH THREADS and move it back up before main
void reverseHash(){

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
    write(newsockfd, &key, 8);

    // Print response sent message
    printf("[%d] Response Sent.\n", requestCounter);

    // Clean up and exit the child process TODO: Change this to work for threads
    close(newsockfd);
    exit(0);
}




// function to read from client
void* reader(void* param)
{
    // Lock the semaphore
    sem_wait(&x);
    readercount++;

    if (readercount == 1)
        sem_wait(&y);

    // Unlock the semaphore
    sem_post(&x);

    // call reverse hash
    reverseHash()

    // Lock the semaphore
    sem_wait(&x);
    readercount--;

    if (readercount == 0) {
        sem_post(&y);
    }

    // Lock the semaphore
    sem_post(&x);

    pthread_exit(NULL);
}






int main(int argc, char *argv[]) {
    signal(SIGINT, terminationHandler);         // Set up signal for graceful termination
    sockfd = socket(AF_INET, SOCK_STREAM, 0);           // Create socket

    // set up semaphore
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

    // Setting the port available in case it is not
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
        perror("ERROR on binding");
        exit(1);
    }

    int NUM_CONNECTIONS;                // number of connections
    NUM_CONNECTIONS = 100;              // set to 100 for testing purposes
    pthread_t tid[NUM_CONNECTIONS];     // array to store each unique ID for threads


    listen(sockfd, NUM_CONNECTIONS);        // Listen for client --> waits for client to make connection with server

    // Declare client address and size
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    // Declare a request counter
    int requestCounter = 0;


    // Begin accepting client connections as concurrent threads
    while (1) {
        // Accept connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        pthread_t thread;

        int fd = *(int *) thread_data;             // close(sockfd);
        free(thread_data)    // free the new socket, is this the same as close?

        // Print request received message
        printf("[%d] Request received.\n", requestCounter);

        // check for error
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1); // exit when the newsockfd < 0 as no more requests
        }

        int choice = 0;
        recv(newSocket, &choice, sizeof(choice), 0);

        if (choice == 1) {
            // Creater readers thread which will eventually call reverseHash()
            if (pthread_create(&readerthreads[i++], NULL,
                               reader, &newSocket)
                != 0)

                // Error in creating thread
                printf("Failed to create thread\n");
        }else{
            printf("Failed to create thread\n");    // we do not need a writer thread for this server
        }


        if (i >= 50) {
            // Update i
            i = 0;

            while (i < 50) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(writerthreads[i++],
                             NULL);
                pthread_join(readerthreads[i++],
                             NULL);
            }

            // Update i
            i = 0;
        }
        pthread_detach(thread);
    }
    return 0;
}



