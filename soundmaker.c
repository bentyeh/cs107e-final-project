#include "soundmaker.h"
#include "circular.h"
#include "armtimer.h"
#include "gpio.h"


#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23


/* prototypes */
int get_time_elapsed();
void set_buttons(int button);


/* hit structure */
struct hit {
	int frequency;
	int volume;
	int time_elapsed;
};

/* Globals */
cir_t* cir;
static int stored_time;
static int first_beat_time;

extern toggle_play; //can i get this from main?
extern toggle_stop; //can i get this from main?

/*
 * initialize the sound maker
 */
void soundmaker_init(void) {
  	

  	set_buttons(START);
  	set_buttons(STOP);
  	set_buttons(PLAY);
  	set_buttons(CLEAR);
	stored_time = 0;
	first_beat_time = 0;
	cir = cir_new();
	cur_frequency = 0;

}

/*
* This function is called every time a beat is hit and needs to be recorded
*** probably should be called by the interrupt handler
*/

soundmaker_record_beat(int i){
	//make new hit and enqueue it
	
	struct hit hit1;
	
	hit1.frequency = i;
	hit1.volume = pwm.get_volume_input();
	hit1.time_elapsed = get_time_elapsed();
	
	//store the pointer to the struct in the circular queue
	
	int *hit1_ptr = &hit1;
	
	cir.enqueue(hit1_ptr);

}

/*
* returns  the hit in the circular queue
*/

hit_t *soundmaker_replay_beat(){
	if(cir_empty(cir)){
		return 0;
	}
	int hptr = cir.dequeue();
	cir_enqueue(hptr);
	return hptr;

}


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



//getter functions for main
int soundmaker_get_frequency(int *hit1){
	return *hit1->frequency;
}

//getter functions for main
int soundmaker_get_volume(int *hit1){
	return *hit1->volume;
}

//getter functions for main
int soundmaker_get_delay(int *hit1){
	return *hit1->delay;
}

//sensor_get_drum should return:	
	// 1 for drum 1
	// 2 for drum 2
	// 3 for drum 3
	// 4 for drum 4
void soundmaker_vector(unsigned pc){
	int i = pwm.sensor_get_drum();
	if(i){	
		if(!toggle_stop)
			soundmaker_record_beat(i);
			//also we should playback the sound on the initial hit
	}
	armtimer_clear_interrupt();
}

void set_buttons(int button){
	gpio_set_function(button, GPIO_FUNC_INPUT); 
  	gpio_set_pullup(button); 
  	gpio_set_input(button);

}

void soundmaker_new_cir(){
	cir = cir_new();
}

void soundmaker_clear_cir(){
	cir_clear(cir);
}
