#include "keyboard.h"
#include "gpio.h"
#include "gpioevent.h"
#include "interrupts.h"


/* Interrupt Handlers */



void interrupt_vector(unsigned pc) {
  if(!gpio_check_event(GPIO_PIN23)){
  	gprof_vector(pc);
  }else{
  	keyboard_vector(pc);
  }                                                                      
}

// The rest of these should never be called
void fast_interrupt_vector(int pc) {}
void reset_vector(int pc) {}
void undefined_instruction_vector(int pc) {}
void software_interrupt_vector(int pc) {}
void prefetch_abort_vector(int pc) {}
void data_abort_vector(int pc) {}