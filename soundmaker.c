#include "soundmaker.h"
#include "circular.h"

/* prototypes */
int get_time_elapsed();


/* hit structure */
struct hit {
	int frequency;
	int volume;
	int time_elapsed;
};

/* Globals */
static cir_t* cir;
int stored_time;
int first_beat_time;

/*
 * initialize the sound maker
 */
void soundmaker_init(void) {
	stored_time = 0;
	first_beat_time = 0;
	cir = cir_new();

}

/*
* This function is called every time a beat is hit and needs to be recorded
*** probably should be called by the interrupt handler
*/

soundmaker_record_beat(){
	//make new hit and enqueue it
	
	struct hit hit1;
	
	hit1.frequency = pwm.get_frequency_input();
	hit1.volume = pwm.get_volume_input();
	hit1.time_elapsed = get_time_elapsed();
	
	//store the pointer to the struct in the circular queue
	
	int *hit1_ptr = &hit1;
	
	cir.enqueue(hit1_ptr);

}

/*
* returns a pointer to the hit in the circular queue
*/

hit_t *soundmaker_replay_beat(){
// 	int *hit1_ptr = cir.dequeue();
// 	struct hit hit1;
// 	hit1 = *hit1_ptr;
	return cir.dequeue();

}
//for output functions how do you want the information parsed?
//make the makefile now




/*
* Records the time elapsed between hits by storing the old time in a stored time 
* global and subtracting off the difference.
*/
int get_time_elapsed(){
	int new_time = timer_get_time();
	if(stored_time == 0){
		first_beat_time = new_time;
		return 0;
	}
	int elapsed_time = (new_time - stored_time);
	stored_time = new_time;
	return elapsed_time;
}


//these should probably go in another c file:::

int soundmaker_get_frequency(int *hit1){
	return *hit1.frequency;
}

int soundmaker_get_volume(int *hit1){
	return *hit1.volume;
}

int soundmaker_get_delay(int *hit1){
	return *hit1.delay;
}

//sensor_get_drum should return:	
	// 1 for drum 1
	// 2 for drum 2
	// 3 for drum 3
	// 4 for drum 4
void soundmaker_vector(int pc){
	int i = sensor_get_drum();
	if(i){
		soundmaker.set_frequency(i);	
	}
}
