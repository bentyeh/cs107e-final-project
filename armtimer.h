#ifndef ARMTIMER_H
#define ARMTIMER_H

/*
 * Functions for Raspberry Pi ARM Timer.
 * 
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 *         Dawson Engler <engler@cs.stanford.edu>
 *    
 * Date: April 20, 2016
 */ 

extern void armtimer_init(unsigned ticks);

extern void armtimer_set_prescalar(unsigned divisor);

extern void armtimer_set_load(unsigned ticks);
extern void armtimer_set_reload(unsigned ticks);

extern unsigned armtimer_get_time(void);

extern void armtimer_enable(void);
extern void armtimer_disable(void);

extern unsigned armtimer_check_overflow(void);

extern void armtimer_enable_interrupt(void);
extern void armtimer_disable_interrupt(void);
extern unsigned armtimer_check_interrupt(void);
extern void armtimer_clear_interrupt(void);

#endif