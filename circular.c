#include "circular.h"
#include "malloc.h"
#include "assert.h"
#include "printf.h"
#include "soundmaker.h"

#define CAPACITY 128


/* hit structure */
struct hit {
	int frequency;
	int volume;
	int time_elapsed;
};

/*cir structure*/
struct cir {
    struct hit buf[CAPACITY];
    unsigned head;
    unsigned tail;
}; 

struct cir cir1;

/* Allocates a new circular buffer, initializes it, and returns a
   pointer to it. */
cir_t *cir_new() {
	hit_t hit_inital;
	for(int i = 0; i < CAPACITY; i++){
        cir1.buf[i] = hit_inital;
    }        
	cir1.head = 0;
	cir1.tail = 0;
	cir_t* cir_ptr = &cir1;
	return cir_ptr;
}

/* Returns 1 if the buffer is empty, 0 otherwise */
int cir_empty(cir_t *cir) {

	if((cir->head) == (cir->tail)){

		return 1;
	}else{
		return 0;
	}
	return 0;
}

/* Returns 1 if the buffer is full (no more elements could fit), 0 otherwise */
int cir_full(cir_t *cir) {
	//cir1 = *cir;
	//if((cir.head == 0 && cir.tail == CAPACITY - 1) || (cir.head > 0 && cir.tail == cir.head - 1)){
	if((cir->tail) - (cir->head) == CAPACITY){
		return 1;
	}else{
		return 0;
	}
	return 0;
}

/* Adds an element to the ring buffer
   Asserts an error if the buffer is full */
void cir_enqueue(cir_t *cir, hit_t x) {

	if(cir_full(cir)){
	}else{

	cir->buf[(cir->tail)%CAPACITY] = x;
	cir->tail = cir->tail + 1;

	}

}

/* Removes an element from the buffer 
   Blocks if the buffer is empty */
hit_t cir_dequeue(cir_t *cir) {
	while(cir_empty(cir)){
		//spin
	}
	
	struct hit return_val = cir->buf[(cir->head)%CAPACITY];
	cir->head = (cir->head) + 1;
	return return_val;
}

void cir_clear(cir_t *cir){
 		
}