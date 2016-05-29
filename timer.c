#include "timer.h"

#define CLO 0x20003004 //lower 32 bits 


/*
 * initialize the timer - does not do anything
 */
void timer_init(void) {
}

/*
 * Returns the current system time in us.
 *
 * @return system time in microseconds
 *
 
 */
unsigned int timer_get_time(void) {
  unsigned volatile int ret_value = *(volatile int*)CLO;
  return ret_value;
}

/*
 * A simple busy loop that delays the program for `n` microseconds.
 *
 * @param n the number of microseconds to busy loop for
 */
void delay_us(unsigned int n) {
  unsigned volatile int start = timer_get_time();
  while (timer_get_time() - start < n) { /* spin */ }
}

/*
 * Delay for `n` seconds.
 *
 * @param n the number of seconds to busy loop for
 */
void delay(unsigned int n) {
  unsigned volatile int i;
  for (i = 0; i < n; i++) {
    delay_us(1000000);
  }
}