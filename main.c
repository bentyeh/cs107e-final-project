#include "sensors.h"
#include "interrupts.h"
#include "timer.h"
#include "circular.h"
#include "soundmaker.h"
#include "pwm.h"
#include "printf.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "armtimer.h"


static void setup_interrupts();

void main(void) {
  gpio_init();
  soundmaker_init();
  armtimer_init();
  armtimer_start(GPROF_TIMER_INTERVAL);
  setup_interrupts();

}


static void setup_interrupts() {
    gpio_detect_falling_edge(GPIO_PIN23);
    gpio_set_pullup(GPIO_PIN23);
    interrupts_enable(INTERRUPTS_GPIO3);
    system_enable_interrupts();
}

main_vector
