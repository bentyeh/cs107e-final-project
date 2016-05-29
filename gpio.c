#include "gpio.h"

#define FSEL0  0x20200000
#define SET0   0x2020001C
#define CLR0   0x20200028
#define GPLEV0 0x20200034 

/*
 * initializes gpio - does not need to do anything
 */
void gpio_init(void) {
}

/*
 * Set GPIO pin number `pin` to the GPIO function `function`. Does not affect
 * other pins.
 * Configures the function of the pin, nothing else.
 *
 * @param pin the pin number to initialize
 * @param function the GPIO function to set for the pin
 */
void gpio_set_function(unsigned int pin, unsigned int function) {
	//first create a pointer with the correct fsel register	
	volatile int* fselp = (int*) FSEL0 + (pin/10); //add the remainder to the memory address to get the fsel
	//bit clear before orring in the function by using a mask to clear bits that are about to be set 
	*fselp &= ~(0b111 << (3 * (pin%10)));
	//dereference the pointer and set equal to the correct function shifted to the right pin
	*fselp |= function << ( 3 * (pin%10)); //configure the pin
}

/*
 * Get the function for GPIO pin number `pin`. Should not affect
 * any registers.
 * Returns the function of a given pin, nothing else.
 *
 * @param pin the pin number to initialize
 */
unsigned int gpio_get_function(unsigned int pin) {
	//get the 3 bits associated with the pin and return
	//first determine which function select register to examine
	volatile int* fselp = (int*) FSEL0 + (pin/10);
	//dereference pointer to get to what is in the address
	unsigned int func = *fselp;
	//shift the relevant bits down
	func = func >> ( 3 * (pin%10));
	//mask the upper bits
	func = func & 0b111;
	//return the 3 relevant bits
	return (func);
}

/*
 * Sets GPIO pin number `pin` to the value `value`. The GPIO pin should be set
 * the output mode for this call to function correctly.
 *
 * @param pin the pin number to set or clear
 * @param value 1 if the pin should be set, 0 otherwise
 */
void gpio_write(unsigned int pin, unsigned int value) {
	//use control flow to check whether to write to the CLR or SET registers
	if(value == 1){
		//first create a pointer which indicates the appropriate register
		volatile int* setp = (int*)  SET0 + (pin/32);
		//then set the appropriate bit to a 1
		*setp = 1 << (pin % 32);
	}else{
		volatile int* clrp = (int*) CLR0 + (pin/32);
		*clrp = 1 << (pin % 32);
	}
}

/*
 * Read GPIO pin number `pin`. 
 *
 * @param pin the pin number to be read
 */
unsigned int gpio_read(unsigned int pin) {
  // TODO: Your code goes here.
	//get the bit associated with the correct pin and return
	//first determine which register to examine
	volatile int* gplevp = (int*) GPLEV0 + (pin/32);
	//then dereference the pointer and store its value
	unsigned int val = *gplevp;
	//then shift the bits in the value
	val = val >> pin;
	//then mask the bits
	val = val & 0b1;
	//then return the bits
	return (val);
}
