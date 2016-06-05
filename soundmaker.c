#include "soundmaker.h"
#include "armtimer.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "sensors.h" 
#include "tone.h"
#include "audio.h"


#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23




/* prototypes */
int get_time_elapsed();
void set_buttons(int button);



/* Globals */
cir_t* cir_record;
cir_t* cir_freeplay;
static int stored_time;
static int first_beat_time;
int value = 0;
int cnt = 0;

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
	cir_record = cir_new();
	cir_freeplay = cir_new();
	
}

/*
* This function is called every time a beat is hit and needs to be recorded
*** probably should be called by the interrupt handler
*/

void soundmaker_record_beat(hit_t hit1){
	//make new hit and enqueue it
	
	hit_t hit2 = hit1;
	hit2.time_elapsed = get_time_elapsed();
	
	cir_enqueue(cir_record, hit2);

}

/*
* returns  the hit in the circular queue
*/

volatile hit_t soundmaker_replay_beat(){
	if(cir_empty(cir_record)){
		// return 0;
	}
	hit_t hit2 = cir_dequeue(cir_record);
	cir_enqueue(cir_record, hit2);
	return hit2;

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
int soundmaker_get_drum(hit_t hit1){
	int i = hit1.drum;
	return i;
}

//getter functions for main
int soundmaker_get_volume(hit_t hit1){
	int i = hit1.volume;
	return i;
}

//getter functions for main
int soundmaker_get_delay(hit_t hit1){
	int i = hit1.time_elapsed;
	return i;
}

	
/*This is a timer interrupt that fires to see if the drums were hit because
we cannot get interrupts directly from SPI*/

void soundmaker_vector(unsigned pc){
	value = 0;
	//clear the interrupt
	armtimer_clear_interrupt();
	
	//set initial values to zero because there might not have been any drum hit
	int sum = 0, drum = 0, num_drums = 0;
	
	//check how hard each drum was hit
	int d0 = sensors_read_value(0);
	int d1 = sensors_read_value(1);
	int d2 = sensors_read_value(2);
	int d3 = sensors_read_value(3);
	
	//add up if multiple drums were hit
	if(d0 > 0){
		sum += d0;
		drum += 1;
		num_drums++;
	}
	if(d1 > 0){
		sum += d1;
		drum += 2;
		num_drums++;
	}
	if(d2 > 0){
		sum += d2;
		drum += 4;
		num_drums++;
	}
	if(d3 > 0){
		sum += d3;
		drum += 8;
		num_drums++;
	}
	
	//average the frequencies between the drum hits and volumes if more than
	//one drum was hit
	//int comb_freq = (drum / num_drums);
	int comb_vol = (sum / num_drums);
	
	//generate a hit from the drums
	hit_t hit1;
	//hit1.frequency = comb_freq;
	value = drum;
	hit1.drum = drum;
	hit1.volume = comb_vol;
	hit1.time_elapsed = 0;
	
	//enqueue the hit into the freeplay circular buffer
	if(d0 > 0 || d1 > 0 || d2 > 0 || d3 > 0){
		cir_enqueue(cir_freeplay, hit1);
		cnt++;
	}
	//enqueue in the recording function
	// if(!toggle_stop)
	// 		soundmaker_record_beat(hit1);		
}



void set_buttons(int button){
	gpio_set_function(button, GPIO_FUNC_INPUT); 
  	gpio_set_pullup(button); 
  	gpio_set_input(button);

}

void soundmaker_new_cir(){
	cir_record = cir_new();
}

void soundmaker_clear_cir(){
	cir_clear(cir_record);
}
