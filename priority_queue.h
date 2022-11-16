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

// Internal function to swap two requests in a priority queue
void swap(priority_queue_t* pq, int i, int j) {
    request_t temp = pq->arr[i];
    pq->arr[i] = pq->arr[j];
    pq->arr[j] = temp;
}

// Internal function to heapify down a request within a priority queue
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

// Internal function to heapify up a request within a priority queue
void heapify_up(priority_queue_t* pq, int i) {
    if (i == 0)
        return;
    int parent = (i - 1) / 2;
    if (compare_requests(&pq->arr[i], &pq->arr[parent]) > 0) {
        swap(pq, i, parent);
        heapify_up(pq, parent);
    }
}

// External function to insert a request into a priority queue and dynamically resize if necessary
void insert(priority_queue_t* pq, request_t* request) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->arr = realloc(pq->arr, sizeof(request_t) * pq->capacity);
    }
    pq->arr[pq->size] = *request;
    heapify_up(pq, pq->size++);
}

// External function to retrieve and delete the highest priority request from a priority queue
request_t extract(priority_queue_t* pq) {
    request_t request = pq->arr[0];
    pq->arr[0] = pq->arr[--pq->size];
    heapify_down(pq, 0);
    return request;
}

#endif // OS_CHALLENGE_WAVY_GANG_PRIORITY_QUEUE_H