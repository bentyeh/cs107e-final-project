#ifndef SENSORS_H
#define SENSORS_H

/*
 * Abstraction for handling input from piezo "knock" vibration sensors.
 * 
 * Author: Eric Cramer <emcramer@stanford.edu>
 * Date: May 29, 2016
 */

void sensors_init(void);
int sensors_read_value(unsigned int channel);

#endif