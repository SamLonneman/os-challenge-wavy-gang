// NOTE: We used some socket logic from https://www.tutorialspoint.com/unix_sockets/index.htm

#include <netdb.h>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "messages.h"

// Declare semaphore and mutex
sem_t requests_in_queue;
pthread_mutex_t lock;

// Declare request type with initializer and comparator
typedef struct request {
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t p;
    int newsockfd;
    int order;
} request_t;
void init_request(request_t* req, uint8_t* hash, uint64_t start, uint64_t end, uint8_t p, int newsockfd, int order) {
    memcpy(req->hash, hash, 32);
    req->start = start;
    req->end = end;
    req->p = p;
    req->newsockfd = newsockfd;
    req->order = order;
}
int compare_requests(request_t* req1, request_t* req2) {
    if (req1->p == req2->p)
        return req2->order - req1->order;
    return req1->p - req2->p;
}

// Declare type for priority queue with initializer, internal functions, external functions, and destructor
typedef struct priority_queue {
    request_t *arr;
    int size;
    int capacity;
} priority_queue_t;
void init_priority_queue(priority_queue_t* pq) {
    pq->arr = malloc(sizeof(request_t) * 100);
    pq->size = 0;
    pq->capacity = 100;
}
void swap(priority_queue_t* pq, int i, int j) {
    request_t temp = pq->arr[i];
    pq->arr[i] = pq->arr[j];
    pq->arr[j] = temp;
}
void heapify_down(priority_queue_t* pq, int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if (left < pq->size && compare_requests(&pq->arr[left], &pq->arr[largest]) > 0)
        largest = left;
    if (right < pq->size && compare_requests(&pq->arr[right], &pq->arr[largest]) > 0)
        largest = right;
    if (largest != i) {
        swap(pq, i, largest);
        heapify_down(pq, largest);
    }
}
void insert(priority_queue_t* pq, request_t* req) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->arr = realloc(pq->arr, pq->capacity * sizeof(request_t));
    }
    pq->arr[pq->size] = *req;
    int i = pq->size;
    pq->size++;
    while (i > 0 && compare_requests(&pq->arr[i], &pq->arr[(i - 1) / 2]) > 0) {
        swap(pq, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}
request_t* extract_max(priority_queue_t* pq) {
    if (pq->size == 0)
        return NULL;
    request_t* max = malloc(sizeof(request_t));
    *max = pq->arr[0];
    pq->arr[0] = pq->arr[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return max;
}
void free_priority_queue(priority_queue_t* pq) {
    free(pq->arr);
}

// Takes in pointer to priority queue
void* worker_thread(void *pq) {

    // Repeatedly take requests from queue and process them
    while (1) {

        // Block while queue is empty
        sem_wait(&requests_in_queue);

        // Pop the request with the highest priority
        pthread_mutex_lock(&lock);
        request_t* request = extract_max(pq);
        pthread_mutex_unlock(&lock);

        // Search for key in given range corresponding to given hash
        uint8_t calculatedHash[32];
        uint64_t key;
        for (key = request->start; key < request->end; key++) {
            SHA256((uint8_t *)&key, 8, calculatedHash);
            if (memcmp(request->hash, calculatedHash, 32) == 0)
                break;
        }

        // Send resulting key back to client
        key = be64toh(key);
        write(request->newsockfd, &key, 8);

        // Close socket
        close(request->newsockfd);
    }
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

    // Initialize the priority queue
    priority_queue_t pq;
    init_priority_queue(&pq);

    // Initialize thread pool
    pthread_t threads[4];
    for (int i = 0; i < 4; i++)
        pthread_create(&threads[i], NULL, worker_thread, &pq);

    // Initialize request counter
    int c = 0;

    // Continuously accept requests to populate the priority queue
    while (1) {

        // Accept connection, and check for error
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // Read in request through new connection
        uint8_t buffer[PACKET_REQUEST_SIZE];
        read(newsockfd, buffer, PACKET_REQUEST_SIZE);

        // Extract components from request
        uint8_t hash[32];
        uint64_t start;
        uint64_t end;
        uint8_t p;
        memcpy(hash, buffer + PACKET_REQUEST_HASH_OFFSET, 32);
        memcpy(&start, buffer + PACKET_REQUEST_START_OFFSET, 8);
        memcpy(&end, buffer + PACKET_REQUEST_END_OFFSET, 8);
        memcpy(&p, buffer + PACKET_REQUEST_PRIO_OFFSET, 1);

        // Convert start and end byte order
        start = htobe64(start);
        end = htobe64(end);

        // Create request object
        request_t request;
        init_request(&request, hash, start, end, p, newsockfd, ++c);

        // Insert request into priority queue
        pthread_mutex_lock(&lock);
        insert(&pq, &request);
        pthread_mutex_unlock(&lock);

        // Up the number of requests in queue
        sem_post(&requests_in_queue);
    }
}
