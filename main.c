#include "sensors.h"
#include "interrupts.h"
#include "timer.h"
#include "circular.h"
#include "soundmaker.h"
#include "pwm.h"
#include "printf.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "armtimer.h"

/* globals*/
static void setup_interrupts();
int toggle_play = 0;
int toggle_stop = 0;
static int toggle_clear = 0;
static int beat_delay = 0;

/*prototypes*/
void main_cycle_sound();

void main(void) {
  gpio_init();
  soundmaker_init();
  armtimer_init();
  armtimer_start(GPROF_TIMER_INTERVAL);
  setup_interrupts();
  while(1){
  	if(toggle_play){
  		//go through playing circular buffer (dequeue and requeue)
  	}
  	if(toggle_stop){
  		//stop playing the circular buffer
  	}
  	if(toggle_clear){
  		soundmaker_clear_cir();
  		toggle_clear = 0;
  		toggle_play = 0;
  	}
  	while(delay(beat_delay)){
  	//update the gl
	
	}
  }

}

/*this changes the delay of the beat_delay according to what the delay from the 
dequeue is. 
it dequeues a value, then it plays the sets the beat to be played after its delay
is completed, then it will requeue the beat when it is done */
void main_cycle_sound(){
	int *hit1_ptr = soundmaker_replay_beat();
		if(hit1_ptr == 0){
			printf("the queue was empty\n");
		}
	beat_delay = soundmaker_get_delay(hit1_ptr);
	//pass the pwm output the volume and frequency
	pwm.play_sound(soundmaker_get_volume(hit1_ptr), soundmaker_get_frequency(hit1_ptr));
	
}


static void setup_interrupts() {
    gpio_detect_falling_edge(START);
    gpio_set_pullup(START);
    gpio_detect_falling_edge(STOP);
    gpio_set_pullup(STOP);
    gpio_detect_falling_edge(PLAY);
    gpio_set_pullup(PLAY);
    gpio_detect_falling_edge(CLEAR);
    gpio_set_pullup(CLEAR);
    interrupts_enable(INTERRUPTS_GPIO3);
    system_enable_interrupts();
}


void main_vector(unsigned pc){
//none of the functionality specified here should actually occur in the handler
//it needs to toggle things that are in a larger while loop that when checked 
//change what is currently happening in the program
  if(pc == (START)){
  	//initialize a new circular buffer
  	 soundmaker_new_cir();
  	 //turn on the recording of interrupts
  	 gpio_check_and_clear_event(START);
  }else if(pc == (STOP)){
  	//stop the recording of interrupts
  	//main_toggle_stop();
  	toggle_stop = 1;
  	gpio_check_and_clear_event(STOP);
  }else if(pc == (PLAY)){
  	//cycle through the circular queue
  	//main_toggle_play();
  	toggle_play = 1;	
  	gpio_check_and_clear_event(PLAY);
  }else if(pc == (CLEAR)){
  	//set all values in the circular queue to zero
  	//main_toggle_clear();
  	toggle_clear = 1;
  	gpio_check_and_clear_event(CLEAR);
  }
  
}


/*toggles so that we know if the circular buffer should be outputting the sound */
// void main_toggle_play(){
// 	if(toggle_play == 0){
// 		toggle_play = 1;
// 	}else{
// 		toggle_play = 0;
// 	}
// }

/* toggles so that we know if we should stop the circular buffer from cycling */
// void main_toggle_stop(){
// 	if(toggle_stop == 0){
// 		toggle_stop = 1;
// 	}else{
// 		toggle_stop = 0;
// 	}
// }

/*toggles so that we clear the circular buffer*/
// void main_toggle_clear(){
// 		if(toggle_clear == 0){
// 		toggle_clear = 1;
// 	}else{
// 		toggle_clear = 0;
// 	}
// }