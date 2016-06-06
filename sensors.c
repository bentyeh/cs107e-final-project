#include "sensors.h"
#include "gpio.h"
#include "spi.h"
#include "printf.h"

#define CHIP_SELECT SPI_CE0
#define CLOCK_DIVIDER 1024

/* Function Prototypes */
static unsigned char set_instruction(unsigned int channel);

void sensors_init(void){
	//Only thinkg to do is init the spi and gpio?
	gpio_init();
	spi_init(CHIP_SELECT, CLOCK_DIVIDER);
}

/*
* Reads sensor data and returns the degree of the 'knock' detected
* Reads single values from the slave as opposed to differentials
* The MCP3008 has seven channels, so only numbers up to 7 may be params
* For the RPi bongos, the channel number corresponds to the drum number
* The max value for a hit on the knock sensor seems to be 1024
*/
int sensors_read_value(unsigned int channel){
	unsigned char instruction[3] = {1,0,0};
	//unsigned char instruction = 0b0; //create the instruction byte
	instruction[1] = 1 << 3; 
	if(!channel){
		//set to hex 0x80
		instruction[1] = 1 << 7;
	}else if(channel > 7){
		//channel does not exist
		return -1; //return -1 to indicate error
	}else{
		//channel exists, bitshift a one into the SPI instruction
		instruction[1] = set_instruction(channel);
	}
	unsigned char values[3] = {0,0,0};
	//Execute the instruction to the slave (a to d)
	spi_transfer(instruction, values, 3); //len is 3 b/c sending and receiving 3 bytes
	int value = values[1] << 8; //need to capture 10 bits from the MCP3008
	value |= values[2];
	return value;
}

/* Helper function to indicate which channel the MCP3008 should read */
static unsigned char set_instruction(unsigned int channel){
	unsigned char instr = 0x80;
	if(channel == 1){
		instr |= (1 << 4); 
	}else if(channel == 2){
		instr |= (1 << 5);
	}else if(channel == 3){
		instr |= (1 << 4);
		instr |= (1 << 5);
	}else if( channel == 4){
		instr |= (1 << 6);
	}else if(channel == 5){
		instr |= (1 << 6);
		instr |= (1 << 4);
	}else if(channel == 6){
		instr |= (1 << 6);
		instr |= (1 << 5);
	}else{
		instr |= (1 << 6);
		instr |= (1 << 5);
		instr |= (1 << 4);
	}
	return instr;
}

int sensors_get_ambient_vibration(int time){
	int ambient_vibrations = 0, counter = 0, samples = 0;
	while(counter < time){
		ambient_vibrations += sensors_read_value(0);
		ambient_vibrations += sensors_read_value(1);
		ambient_vibrations += sensors_read_value(2);
		ambient_vibrations += sensors_read_value(3);
		ambient_vibrations += sensors_read_value(4);
		ambient_vibrations += sensors_read_value(5);
		ambient_vibrations += sensors_read_value(6);
		//ambient_vibrations += sensors_read_value(7); //8th channel is unused
		samples += 7;
		counter++;
	}
	return ambient_vibrations / samples;
}