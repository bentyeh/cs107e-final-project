#ifndef TIMER_H
#define TIMER_H

/*
 * Hardware abstractions for a Raspberry Pi timer.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *         Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Jan 24, 2016
 */

void timer_init(void);
unsigned int timer_get_time(void);

void delay_us(unsigned int usec);
void delay_ms(unsigned int msec);
void delay(unsigned int sec);

void until_us(unsigned int usec);

#endif