#ifndef SENSORS_H
#define SENSORS_H

/*
 * Abstraction for handling input from piezo "knock" vibration sensors.
 * 
 * Author: Eric Cramer <emcramer@stanford.edu>
 * Date: May 29, 2016
 */

 void sensors_init(void);

enum{
	SENSOR_PIN_FIRST = 5;
	SENSOR_PIN1 = 5;
	SENSOR_PIN2 = 6;
	SENSOR_PIN3 = 13;
	SENSOR_PIN4 = 19;
	SENSOR_PIN5 = 26;
	SENSOR_PIN_LAST = 26;
}

int sensors_get_sensor_number(void);



#endif