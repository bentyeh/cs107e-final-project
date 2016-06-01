#include "sensors.h"
#include "gpio.h"
#include "spi.h"

#define CHIP_SELECT SPI_CE0
#define CLOCK_DIVIDER 1024

/* Function Prototypes */
static void set_instruction(unsigned int channel, unsigned char instr);

void sensors_init(void){
	//TODO: implement
	spi_init(CHIP_SELECT, CLOCK_DIVIDER);
}

/*
* Reads sensor data and returns the degree of the 'knock' detected
* Reads single values from the slave as opposed to differentials
* The MCP3008 has seven channels, so only numbers up to 7 may be params
*/
unsigned int sensors_read_value(unsigned int channel){
	unsigned char instruction = 0b0; //create the instruction byte
	instruction = 1 << 4;
	if(!channel){
		//channel is 0, do nothing
		continue;
	}else if(channel > 7){
		//channel does not exist
		return -1; //return -1 to indicate error
	}else{
		//channel exists, bitshift a one into the SPI instruction
		set_instruction(channel, instruction);
	}
	//Execute the instruction to the slave (a to d)
	spi_transfer(); //what is tx, rx, and len?
}

static void set_instruction(unsigned int channel, unsigned char instr){
	switch(channel){
		case 1:
			instr |= (1 << 1); 
			break;
		case 2:
			instr |= (1 << 2);
			break;
		case 3:
			instr |= (1 << 1);
			instr |= (1 << 2);
			break;
		case 4:
			instr |= (1 << 3);
			break;
		case 5:
			instr |= (1 << 1);
			instr |= (1 << 3);
			break;
		case 6:
			instr |= (1 << 2);
			instr |= (1 << 3);
			break;
		case 7:
			instr |= (1 << 1);
			instr |= (1 << 2);
			instr |= (1 << 3);
			break;
		default: //do nothing
			break;
	}
}