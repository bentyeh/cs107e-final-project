#include "circular.h"
#include "malloc.h"
#include "assert.h"

#define CAPACITY 128

extern void malloc_init();

// hit structure
struct hit {
    int frequency;
    int volume;
    int time_elapsed;
};

struct cir {
    hit_t buf[CAPACITY];
    unsigned head;
    unsigned tail;
};

/* Allocates a new circular buffer, initializes it, and returns a
   pointer to it. */
cir_t *cir_new() {
    // malloc_init();
    cir_t *cirptr = malloc(sizeof(cir_t));
    cirptr->head = 0;
    cirptr->tail = 0;
    return cirptr;
}

/* Returns 1 if the buffer is empty, 0 otherwise */
int cir_empty(cir_t *cirptr) {
    return cirptr->head == cirptr->tail;
}

/* Returns 1 if the buffer is full (no more elements could fit), 0 otherwise */
int cir_full(cir_t *cirptr) {
    return ((cirptr->tail + 1) % CAPACITY) == (cirptr->head);
}

/* Adds an element to the ring buffer
   Asserts an error if the buffer is full */
void cir_enqueue(cir_t *cirptr, hit_t x) {
    if(!cir_full(cirptr)) {
        cirptr->buf[cirptr->tail] = x;
        cirptr->tail = (cirptr->tail + 1) % CAPACITY;
    }
}

/* Removes an element from the buffer 
   Blocks if the buffer is empty */
hit_t cir_dequeue(cir_t *cirptr) {
    while(cir_empty(cirptr)) {}
    hit_t val = cirptr->buf[cirptr->head];
    cirptr->head = (cirptr->head + 1) % CAPACITY;
    return val;
}

void cir_clear(cir_t *cirptr) {
    while(!cir_empty(cirptr)) {
        cir_dequeue(cirptr);
    }
}