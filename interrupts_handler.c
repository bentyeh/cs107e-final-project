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
  		soundmaker_vector(START);
  }else if(gpio_check_event(STOP)){
  		soundmaker_vector(STOP);
  }else if(gpio_check_event(PLAY)){
  		soundmaker_vector(PLAY);
  }else if(gpio_check_event(CLEAR)){
  		soundmaker_vector(CLEAR);
  }else{
  		timer_vector(pc);
  }                                                                                                                  
}

// The rest of these should never be called
void fast_interrupt_vector(int pc) {}
void reset_vector(int pc) {}
void undefined_instruction_vector(int pc) {}
void software_interrupt_vector(int pc) {}
void prefetch_abort_vector(int pc) {}
void data_abort_vector(int pc) {}