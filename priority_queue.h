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

// External function to insert a request into a priority queue and dynamically resize if necessary
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

// External function to retrieve the highest priority request from a priority queue
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

#endif // OS_CHALLENGE_WAVY_GANG_PRIORITY_QUEUE_H