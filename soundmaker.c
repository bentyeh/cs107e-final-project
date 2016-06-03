#include "soundmaker.h"
#include "circular.h"
#include "armtimer.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "sensors.h" 


#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23

/* Defines for the frequencies of different drums */
#define TOM_FREQ 120 //full
#define CYMBAL_FREQ 200 //clank
#define KICK_FREQ 60 //thump
#define BONGO_FREQ 80 //approx
#define CONGA_FREQ 100 //approx
#define HIGH_HAT_FREQ 10 //sizzle


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
int value = 0;

extern int toggle_play; //can i get this from main?
extern int toggle_stop; //can i get this from main?

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

}

/*
* This function is called every time a beat is hit and needs to be recorded
*** probably should be called by the interrupt handler
*/

void soundmaker_record_beat(int drum, int i){
	//make new hit and enqueue it
	
	struct hit hit1;
	
	hit1.frequency = drum;
	hit1.volume = i;  	//WHAT IS THE VOLUME INPUT HERE?
	hit1.time_elapsed = get_time_elapsed();
	
	//store the pointer to the struct in the circular queue
	
	struct hit *hit1_ptr = &hit1;
	
	cir_enqueue(cir, hit1_ptr);

}

/*
* returns  the hit in the circular queue
*/

hit_t *soundmaker_replay_beat(){
	if(cir_empty(cir)){
		return 0;
	}
	volatile struct hit *hptr = cir_dequeue(cir);
	cir_enqueue(cir, hptr);
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
int soundmaker_get_frequency(struct hit *hit1){
	struct hit hit2 = *hit1;
	int i = hit2.frequency;
	return i;
}

//getter functions for main
int soundmaker_get_volume(struct hit *hit1){
	struct hit hit2 = *hit1;
	int i = hit2.volume;
	return i;
}

//getter functions for main
int soundmaker_get_delay(struct hit *hit1){
	struct hit hit2 = *hit1;
	int i = hit2.time_elapsed;
	return i;
}

//sensor_get_drum should return:	
	// 1 for drum 1
	// 2 for drum 2
	// 3 for drum 3
	// 4 for drum 4
void soundmaker_vector(unsigned pc){
	int i = 0, drum = 0;
	if(!(sensors_read_value(0) <= 100)){
		i = sensors_read_value(0);
		drum = TOM_FREQ;
		
	}// else if(sensors_read_value(1)){
// 		i = sensors_read_value(1);
// 		drum = TOM_FREQ;
// 	}else if(sensors_read_value(2)){
// 		i = sensors_read_value(2);
// 		drum = KICK_FREQ;
// 	}else if(sensors_read_value(3)){
// 		i = sensors_read_value(3);
// 		drum = BONGO_FREQ;
// 	}
	if(i){	
		//play back beat without storing it
		audio_send_tone(WAVE_SINE, drum, i);
		if(!toggle_stop)
			soundmaker_record_beat(drum, i);
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
