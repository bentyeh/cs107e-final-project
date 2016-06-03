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
#include "audio.h"
#include "tone.h"
#include "drumimage.h"

#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23
#define GPROF_TIMER_INTERVAL 0x10
#define DURATION 1000

/* globals*/
static void setup_interrupts();
int toggle_play = 0;
int toggle_stop = 1;
static int toggle_clear = 0;
static int beat_delay = 0;

extern int value;

/*prototypes*/
void main_cycle_sound();

void main(void) {
  audio_init();
  sensors_init();
  gpio_init();
  soundmaker_init();
 // drumimage_init((unsigned width, unsigned height, unsigned num));
  armtimer_init();
  armtimer_start(GPROF_TIMER_INTERVAL);
  setup_interrupts();
  while(1){
	if(!cir_empty(cir_freeplay)){
		//play sound
		hit_t hit1 = cir_dequeue(cir_freeplay);
		audio_send_tone(WAVE_SINE, hit1.frequency, hit1.volume, DURATION);
		//beat_drum(drum_num, DURATION);
	}
	if(toggle_play){
		//cycle through the stored buffer and figure out how to deal with delays
		while(!toggle_stop){
			main_cycle_sound();
		}
		
	}
	
  	printf("value: %d\n", value);
  }
	
}



/*this changes the delay of the beat_delay according to what the delay from the 
dequeue is. 
it dequeues a value, then it plays the sets the beat to be played after its delay
is completed, then it will requeue the beat when it is done */
void main_cycle_sound(){
	hit_t play_out = cir_dequeue(cir_record);
	cir_enqueue(cir_record, play_out);
	int delay_time = play_out.time_elapsed;
	delay(delay_time);
	audio_send_tone(WAVE_SINE, play_out.frequency, play_out.volume, DURATION);
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
  	 toggle_stop = 0;
  	// turn on the recording of interrupts
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