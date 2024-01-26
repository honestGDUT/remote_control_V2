#include <stdint.h>
#include <stdbool.h>
#include "queue.h"

void initQueue(CircularQueue* queue, uint8_t *data, uint32_t len) {
    queue->buffer = data;
    queue->len = len;
    queue->head = 0;
    queue->tail = 0;
    queue->isFull = false;
}

bool isFull(const CircularQueue* queue) {
    return queue->isFull;
}

bool isEmpty(const CircularQueue* queue) {
    return (queue->head == queue->tail) && !queue->isFull;
}

void enqueue(CircularQueue* queue, uint8_t data) {
    queue->buffer[queue->tail] = data;

    if (isFull(queue)) {
        // Move head to discard the oldest data
        queue->head = (queue->head + 1) % queue->len;
    }

    queue->tail = (queue->tail + 1) % queue->len;

    // Check if the queue is full after the enqueue operation
    queue->isFull = (queue->tail == queue->head);
}

uint8_t dequeue(CircularQueue* queue) {
    uint8_t data = 0;

    if (!isEmpty(queue)) {
        data = queue->buffer[queue->head];
        queue->head = (queue->head + 1) % queue->len;
        queue->isFull = false;
    }

    return data;
}
