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


static void setup_interrupts();

void main(void) {
  gpio_init();
  soundmaker_init();
  armtimer_init();
  armtimer_start(GPROF_TIMER_INTERVAL);
  setup_interrupts();

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
  	 //turn on the recording of interrupts
  }else if(pc == (STOP)){
  	//stop the recording of interrupts
  }else if(pc == (PLAY)){
  	//cycle through the circular queue	
  }else if(pc == (CLEAR)){
  	//set all values in the circular queue to zero

}
