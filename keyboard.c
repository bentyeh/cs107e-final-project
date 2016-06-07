#include "gpio.h"
#include "ps2.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "uart.h"
#include "timer.h"
#include "printf.h"

const unsigned CLK  = GPIO_PIN23;
const unsigned DATA = GPIO_PIN24;

void wait_for_falling_clock_edge() {
    while(gpio_read(CLK) == 0) {}
    while(gpio_read(CLK) == 1) {}
}

void keyboard_init(void) {
    gpio_set_function(CLK, GPIO_FUNC_INPUT);
    gpio_set_pullup(CLK);
 
    gpio_set_function(DATA, GPIO_FUNC_INPUT);
    gpio_set_pullup(DATA);
}

int keyboard_read_scancode(void) {
    int i, c, tmp;
    
    c = 0;
    
    wait_for_falling_clock_edge();
    if (gpio_read(DATA) == 0) { // Check for start bit (0), then collect data bits
        // Collect 8 data bits, least significant bit first
        for (i = 0; i < 8; i++) {
            c >>= 1;
            wait_for_falling_clock_edge();
            tmp = gpio_read(DATA);
            c += (tmp << 7);
        }
    }

    return c;
}