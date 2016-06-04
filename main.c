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
#include "drumimage.h"

#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23
#define GPROF_TIMER_INTERVAL 0x10
#define DURATION 1000
#define WIDTH 800
#define HEIGHT 600
#define NUM_DRUMS 4

/* drumsssss */
#define DRUM_1 TOM_FREQ
#define DRUM_2 KICK_FREQ
#define DRUM_3 BONGO_FREQ
#define DRUM_4 CONGA_FREQ

// Global variables
int toggle_play = 0;
int toggle_stop = 1;
static int toggle_clear = 0;
static int beat_delay = 0;
int DRUM_FREQ[NUM_DRUMS] = {DRUM_1, DRUM_2, DRUM_3, DRUM_4};

// External global variables
extern int value;
extern cir_t* cir_record;
extern cir_t* cir_freeplay;

// Function prototypes
void main_cycle_sound();
static void setup_interrupts();

void main(void) {
    audio_init();
    sensors_init();
    gpio_init();
    soundmaker_init();
    drumimage_init(WIDTH, HEIGHT, NUM_DRUMS);
    armtimer_init();
    armtimer_start(GPROF_TIMER_INTERVAL);
    setup_interrupts();

    int freq1, freq2;

    while(1){
        // Freeplay
        if(!cir_empty(cir_freeplay)) {
            // reset frequencies
            freq1 = 0;
            freq2 = 0;

            // play sound
            hit_t hit1 = cir_dequeue(cir_freeplay);
            for(int i = 0; i < NUM_DRUMS; i++) {
                if(hit1.drum & (1<<i)) {
                    if(!freq1) {
                        freq1 = DRUM_FREQ[i];
                    }
                    else if(!freq2) {
                        freq2 = DRUM_FREQ[i];
                    }
                    else {
                        break;
                    }
                    printf("i : %d\n", i);
                    beat_drum(i, DURATION);
                    audio_send_tone(WAVE_SINE, freq1, hit1.volume, DURATION);
                    break;
                }
            }

            //average_sine(freq1, freq2);
            // audio_send_tone(WAVE_SINE, , hit1.volume, DURATION);
    		// beat_drum(drum_num, DURATION);
        }

        // Playback
        if(toggle_play) {
    		// cycle through the stored buffer and figure out how to deal with delays
            while(!toggle_stop){
                main_cycle_sound();
            }
        }

        // Debugging - print out sensor read value
        if(value != 0)
        printf("value: %d\n", value);
    }
}



/*this changes the delay of the beat_delay according to what the delay from the 
dequeue is. 
it dequeues a value, then it plays the sets the beat to be played after its delay
is completed, then it will requeue the beat when it is done */
void main_cycle_sound(){
	hit_t play_out = cir_dequeue(cir_record);
	cir_enqueue(cir_record, play_out);
	int delay_time = play_out.time_elapsed;
	delay(delay_time);
	audio_send_tone(WAVE_SINE, play_out.drum, play_out.volume, DURATION);
}


static void setup_interrupts() {
    gpio_detect_falling_edge(START);
    gpio_set_pullup(START);
    gpio_detect_falling_edge(STOP);
    gpio_set_pullup(STOP);
    gpio_detect_falling_edge(PLAY);
    gpio_set_pullup(PLAY);
    gpio_detect_falling_edge(CLEAR);
    gpio_set_pullup(CLEAR);
    interrupts_enable(INTERRUPTS_GPIO3);
    system_enable_interrupts();
}


//none of the functionality specified here should actually occur in the handler
//it needs to toggle things that are in a larger while loop that when checked 
//change what is currently happening in the program

void main_vector(unsigned pc){
    if(pc == (START)){
      	//initialize a new circular buffer
        soundmaker_new_cir();
        toggle_stop = 0;
      	// turn on the recording of interrupts
        gpio_check_and_clear_event(START);
        }else if(pc == (STOP)){
          	//stop the recording of interrupts
           toggle_stop = 1;
           gpio_check_and_clear_event(STOP);
        }else if(pc == (PLAY)){
          	//cycle through the circular queue
           toggle_play = 1;	
           gpio_check_and_clear_event(PLAY);
        }else if(pc == (CLEAR)){
          	//set all values in the circular queue to zero
           toggle_clear = 1;
           gpio_check_and_clear_event(CLEAR);
    }
}