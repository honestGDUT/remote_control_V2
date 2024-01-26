#pragma once

#include "main.h"
#include <stdbool.h>


typedef struct {
    uint8_t* buffer;
    uint32_t len;
    uint8_t head;
    uint8_t tail;
    bool isFull;
} CircularQueue;

void initQueue(CircularQueue* queue, uint8_t *data, uint32_t len);
void enqueue(CircularQueue* queue, uint8_t data);
uint8_t dequeue(CircularQueue* queue);
