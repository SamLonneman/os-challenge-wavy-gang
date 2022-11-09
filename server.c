
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>

#include <openssl/sha.h>

#include <signal.h>
#include <unistd.h>

#include "messages.h"

#include <arpa/inet.h>

#include <pthread.h>
#include <sys/socket.h>

#include <semaphore.h>  // included from https://www.geeksforgeeks.org/handling-multiple-clients-on-server-with-multithreading-using-socket-programming-in-c-cpp/

// Experiment: Implementing threads with the use of Semaphores to facilitate a stable multiprocessing environment
// semaphore count indicates the number of free resources
// can potentially be adjusted to allow max priority access to free resources
// threads open in all free resources

// Hypothesis: Theoretically this should be faster than the milestone which was process based however experiment 1 makes us question this
// unsure whether the use of semaphores specifically in the implementation of threads will increase or decrease speed

// Multithreading: singular process with multiple threads of control
// Threads share resources (same address space) and should therefore be more efficient than processes
// Faster to switch and creat/terminate


//int sem_post(sem_t *sem); // increments the semaphore currently being pointed to
//int sem_wait(sem_t *sem); // decrements the semaphore currently being pointed to
sem_t x, y;
int readercount = 0;
pthread_t readerthreads[100];
pthread_t tid;
int requestCounter = 0;
// set up semaphore
sem_init(&x, 0, 1);
sem_init(&y, 0, 1);


void * reverseHash(void *arg){

    // Declare request components
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t p;
    int newsockfd;

    // Extract components from request
    memcpy(hash, arg + PACKET_REQUEST_HASH_OFFSET, 32);
    memcpy(&start, arg + PACKET_REQUEST_START_OFFSET, 8);
    memcpy(&end, arg + PACKET_REQUEST_END_OFFSET, 8);
    memcpy(&p, arg + PACKET_REQUEST_PRIO_OFFSET, 1);
    memcpy(&newsockfd, arg + PACKET_REQUEST_PRIO_OFFSET, sizeof(int));

    free(arg);

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

    // Clean up and exit the thread
    close(newsockfd);
    pthread_kill;
}




// function to read from client
void* reader(void* param)
{
    printf("[%d] Request received to reader.\n", requestCounter);

    // Lock the semaphore
    sem_wait(&x);       // this is where we are errorring
    printf("line 103");
    readercount++;
    printf("line 105");

    if (readercount == 1)
        sem_wait(&y);
    printf("line 109");

    // Unlock the semaphore
    sem_post(&x);
    printf("line 113");

    printf("Before reverse hash call");
    // call reverse hash while in reader mode
    reverseHash(param);
    printf("After reverse hash call");

    // Lock the semaphore
    sem_wait(&x);
    readercount--;

    if (readercount == 0) {
        sem_post(&y);
    }

    // Lock the semaphore
    sem_post(&x);

    // kill the thread
    pthread_kill;
}






int main(int argc, char *argv[]) {

    int sockfd;               // holds the return of the socket function
    int newSockFd;

    socklen_t addr_size;


    // Initialize socket structure
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);           // Create socket

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    printf("Inside Alana's Server \n");


    // Setting the port available in case it is not
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }

    // Bind to host address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR when accepting");
        exit(1);
    }





    // Declare client address and size
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr); // ### should NUM_CONNECTIONS = clilen?



    int NUM_CONNECTIONS;                // number of connections
    NUM_CONNECTIONS = 55;              // set to 50 for testing purposes
    pthread_t tid[NUM_CONNECTIONS];     // array to store each unique ID for threads


    listen(sockfd, NUM_CONNECTIONS);        // Listen for client --> waits for client to make connection with server


    // initialise i to 0 (will act as a thread counter)
    int i = 0;

    // Begin accepting client connections as concurrent threads
    while (1) {

        // Accept connection ( will take the first in the queue)
        int newSockFd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        //int newSocket;

        // check for error
        if (newSockFd < 0) {
            perror("ERROR on accept");
            exit(1); // exit when the newsockfd < 0 as no more requests
        }


        // Print request received message and increment request counter
        printf("[%d] Request received.\n", requestCounter);
        requestCounter++;

        int choice = 0;         // choice = 1 for reader or 2 for writer but this client only sends things to be read
        recv(newSockFd,&choice, sizeof(choice), 0); // ### this is where the problem is

        printf("%d size of choice.\n", sizeof(choice));
        printf("%d choice.\n", choice);

        pthread_create(&readerthreads[i++], NULL, reader, &newSockFd);



        if (i >= NUM_CONNECTIONS) {
            i = 0;

            while (i < 50) {
                // Suspend execution of the calling thread until the target thread terminates
                pthread_join(readerthreads[i++],
                             NULL);
            }
            // Update i
            i = 0;
        }
    }
    return 0;
}






