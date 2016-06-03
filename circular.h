#ifndef CIRCULAR_H
#define CIRCULAR_H

/*
 * Circular buffer data structure. Provides a queue interface.
 *
 * Safe if 1 reader (main program, usually) and 1 writer (interrupt handler,
 * usually) are accessing simultaneously.
 */

typedef volatile struct cir cir_t;

// Allocates and initializes a new circular buffer. To set up a circular buffer
// in your code:
//
//     cir_t *cir = cir_new();
//
// Notice that this interface is slightly different from the _init exposed by
// other libpi modules. This _new pattern allows a user to have as many circular
// queues as they need, like objects in Java. It also means that users of this
// module don't need to know the implementation details (like size) of cir_t,
// since they just keep a pointer.
cir_t *cir_new();

// Checks if cir is empty.
int cir_empty(cir_t *cir);

// Checks if cir is full.
// If this returns 1, then a call to cir_enqueue will cause an error.
int cir_full(cir_t *cir);

// Puts elem at the back of the queue.
void cir_enqueue(cir_t *cir, struct hit*);

// Returns an element from the front of the queue; if there
// is no element in the queue, spins and waits  (blocks) 
// until one is enqueued, then returns it
hit_t *cir_dequeue(cir_t *cir);


//Clears the circular buffer by setting all the values equal to zero
void cir_clear(cir_t *cir);

#endif