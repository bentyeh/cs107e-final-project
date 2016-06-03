#include "circular.h"
#include "malloc.h"
#include "assert.h"
#include "printf.h"
#include "soundmaker.h"

#define CAPACITY 128

/*cir structure*/
struct cir {
    int* buf[CAPACITY];
    unsigned head;
    unsigned tail;
}; 

/* hit structure */
struct hit {
	int frequency;
	int volume;
	int time_elapsed;
};

struct cir cir1;

/* Allocates a new circular buffer, initializes it, and returns a
   pointer to it. */
cir_t *cir_new() {
	
	for(int i = 0; i < CAPACITY; i++){
        cir1.buf[i] = 0;
    }        
	cir1.head = 0;
	cir1.tail = 0;
	cir_t* cir_ptr = &cir1;
	return cir_ptr;
}

/* Returns 1 if the buffer is empty, 0 otherwise */
int cir_empty(cir_t *cir) {
	//printf("i: %d\n", i);
	if((cir->head) == (cir->tail)){
// 		printf("cir empty\n");
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
void cir_enqueue(cir_t *cir, volatile struct hit* x) {
// 	printf("init head: %d\n", (cir->head) % CAPACITY);
// 	printf("init tail: %d\n", (cir->tail) % CAPACITY);
	//cir1 = *cir;
	if(cir_full(cir)){
// 	 printf("error buffer is full\n");
	}else{

	cir->buf[(cir->tail)%CAPACITY] = (int*)x;
	cir->tail = cir->tail + 1;
	
// 	printf("inserting at: %d\n", (cir->tail) % CAPACITY);
// 	printf("inserting: %d\n", x);
// 	printf("buf[cir->tail]: %d\n", cir->buf[(cir->tail - 1)%CAPACITY]);
	}
// 	printf("final head: %d\n", (cir->head) % CAPACITY);
// 	printf("final tail: %d\n", (cir->tail) % CAPACITY);
// 	printf("final headn: %d\n", (cir->head));
// 	printf("final tailn: %d\n", (cir->tail));
}

/* Removes an element from the buffer 
   Blocks if the buffer is empty */
hit_t *cir_dequeue(cir_t *cir) {
	//cir1 = *cir;
	while(cir_empty(cir)){
		//spin
	}
	
	struct hit* return_val = (struct hit*)cir->buf[(cir->head)%CAPACITY];
	cir->head = (cir->head) + 1;
// 	if(cir.head == cir.tail){
// 		//IF THE BUFFER HAS BEEN EMPTIED RESET TO ZERO
// 		cir.head = 0;
// 		cir.tail = 0;
// 	}else if(cir.head%CAPACITY == CAPACITY - 1){
// 		//IF THE BUFFER IS NEARLY FULL RESET HEAD
// 		cir.head = 0;
// 	}else{
		
//	}
// 	printf("RETURNING FROM at: %d\n", (cir->head - 1) % CAPACITY);
// 	printf("returning: %d\n", return_val);
// 	printf("buf[cir->head]: %d\n", cir->buf[(cir->head - 1)%CAPACITY]);
// 	printf("final head: %d\n", (cir->head) % CAPACITY);
// 	printf("final tail: %d\n", (cir->tail) % CAPACITY);
// 	if(cir_empty(cir)){
// 		printf("empty\n");
// 	}
// 	
	return return_val;
}

void cir_clear(cir_t *cir){
// 	while(!cir_empty()){
// 		int deq = cir_dequeue(cir);
// 		//now we get the memory location of the struct
// 		//set the frequency, volume, and delay to be zero
// 		*deq->frequency = 0;
// 		*deq->volume = 0;
// 		*deq->delay = 0;
// 	}
// 		
}