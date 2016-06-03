#ifndef ARMTIMER_H
#define ARMTIMER_H


void armtimer_init(void);
unsigned armtimer_get_time(void);
void armtimer_start(unsigned interval);
void armtimer_clear_interrupt();

#endif