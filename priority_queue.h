#ifndef OS_CHALLENGE_WAVY_GANG_PRIORITY_QUEUE_H
#define OS_CHALLENGE_WAVY_GANG_PRIORITY_QUEUE_H

#include "request.h"

// Object-oriented priority queue structure for requests
typedef struct priority_queue {
    request_t *arr;
    int size;
    int capacity;
} priority_queue_t;

// Constructor
void init_priority_queue(priority_queue_t* pq) {
    pq->arr = malloc(sizeof(request_t) * 100);
    pq->size = 0;
    pq->capacity = 100;
}

// Helper function to swap two requests in a priority queue
void swap(priority_queue_t* pq, int i, int j) {
    request_t temp = pq->arr[i];
    pq->arr[i] = pq->arr[j];
    pq->arr[j] = temp;
}

// External function to insert a request into a priority queue
void insert(priority_queue_t* pq, request_t* request) {
    // Resize if full
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->arr = realloc(pq->arr, sizeof(request_t) * pq->capacity);
    }
    // Insert request in last spot and bubble up
    pq->arr[pq->size++] = *request;
    int i = pq->size - 1;
    int parent = (i - 1) / 2;
    while (i > 0 && compare_requests(&pq->arr[i], &pq->arr[parent]) > 0) {
        swap(pq, i, parent);
        i = parent;
    }
}

// External function to retrieve and delete the highest priority request from a priority queue
request_t extract(priority_queue_t* pq) {
    // Save the highest priority request for return
    request_t request = pq->arr[0];
    // Move the last request to the top
    pq->arr[0] = pq->arr[pq->size - 1];
    pq->size--;
    // Bubble down
    int i = 0;
    while (i < pq->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int max = i;
        if (left < pq->size && compare_requests(&pq->arr[left], &pq->arr[max]) > 0)
            max = left;
        if (right < pq->size && compare_requests(&pq->arr[right], &pq->arr[max]) > 0)
            max = right;
        if (max == i)
            break;
        swap(pq, i, max);
        i = max;
    }
    // Return saved request
    return request;
}


#endif // OS_CHALLENGE_WAVY_GANG_PRIORITY_QUEUE_H