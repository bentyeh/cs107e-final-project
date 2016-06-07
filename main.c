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
#include "audio.h"
#include "tone.h"
#include "gl_graph.h"
#include "pitch.h"

#define TIMER_INTERVAL 0x100
#define DURATION 100000
#define WIDTH 800
#define HEIGHT 600
#define NUM_KEYS 3

/* drumsssss */
// #define KEY_A TOM_FREQ
// #define KEY_B KICK_FREQ
// #define KEY_C BONGO_FREQ
// #define DRUM_4 CONGA_FREQ

// Global variables
int toggle_play = 0;
int toggle_stop = 1;
static int toggle_clear = 0;
static int beat_delay = 0;
int KEY_FREQ[NUM_KEYS] = {NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6};

// External global variables
extern int value;
extern cir_t* cir_record;
extern cir_t* cir_freeplay;
extern int cnt;

// Function prototypes
void main_cycle_sound();
static void setup_interrupts();

static void main_init(){
    audio_init();
    sensors_init();
    gpio_init();
    soundmaker_init(NUM_KEYS);
    setup_interrupts();
    graph_init(WIDTH, HEIGHT, NUM_KEYS);
}

void main(void) {
    main_init();

    while(1) {
        hit_t hit1 = cir_dequeue(cir_freeplay);
        for(int i = 0; i < NUM_KEYS; i++) {
            printf("sensor %d: %d  ", i, hit1.value_array[i]);
        }
        // graph_values(NUM_KEYS, cir_dequeue(cir_freeplay));
        // cir_clear(cir_freeplay);
    }
}

static void setup_interrupts() {
    armtimer_init();
    armtimer_start(TIMER_INTERVAL);
    interrupts_enable(INTERRUPTS_GPIO3);
    system_enable_interrupts();
}