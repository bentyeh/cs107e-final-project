#include "keyboard.h"
#include "gpio.h"
#include "gpioevent.h"
#include "interrupts.h"
#include "soundmaker.h"


#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23

/* Interrupt Handlers */


void interrupt_vector(unsigned pc) {
//if it isn't a button, then check the drums

  if(gpio_check_event(START)){
  		main_vector(START);
  }else if(gpio_check_event(STOP)){
  		main_vector(STOP);
  }else if(gpio_check_event(PLAY)){
  		main_vector(PLAY);
  }else if(gpio_check_event(CLEAR)){
  		main_vector(CLEAR);
  }else{
  	//its a drum!!!
  	//this is a timer interrupt
  		//soundmaker_vector(pc);
  }                                                                                                                  

  // if(gpio_check_event(START)){
  // 		main_vector(START);
  // }else if(gpio_check_event(STOP)){
  // 		main_vector(STOP);
  // }else if(gpio_check_event(PLAY)){
  // 		main_vector(PLAY);
  // }else if(gpio_check_event(CLEAR)){
  // 		main_vector(CLEAR);
  // }else{
  // 	//its a drum!!!
  // 	//this is a timer interrupt
  // 		soundmaker_vector(pc);
  // }                                                                                                                  

}

// The rest of these should never be called
void fast_interrupt_vector(int pc) {}
void reset_vector(int pc) {}
void undefined_instruction_vector(int pc) {}
void software_interrupt_vector(int pc) {}
void prefetch_abort_vector(int pc) {}
void data_abort_vector(int pc) {}