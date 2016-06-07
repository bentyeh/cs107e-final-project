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
#include "pitch.h"

#define START GPIO_PIN20
#define STOP GPIO_PIN21
#define PLAY GPIO_PIN22
#define CLEAR GPIO_PIN23
#define GPROF_TIMER_INTERVAL 0x10
#define DURATION 100000
#define WIDTH 800
#define HEIGHT 600
#define NUM_KEYS 7

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
    drumimage_init(WIDTH, HEIGHT, NUM_KEYS);
    armtimer_init();
    armtimer_start(GPROF_TIMER_INTERVAL);
    setup_interrupts();
}

void main(void) {
    main_init();
    int freq1, freq2;
    while(1){
        // Freeplay
        //if(!cir_empty(cir_freeplay)) {
            // reset frequencies
            freq1 = 0;
            freq2 = 0;
            // play sound
            hit_t hit5 = cir_dequeue(cir_freeplay);
            for(int i = 0; i < NUM_KEYS; i++) {
            	if(hit5.drum == i){
            		beat_drum(i, DURATION);
            		freq1 = KEY_FREQ[i];
            		audio_send_mix_wave(freq1, 0, hit5.volume);
            	}
            
            }
            // for(int i = 0; i < NUM_KEYS; i++) {
//                 if(hit5.drum & (1<<i)) {
//                     if(!freq1) {
//                         freq1 = KEY_FREQ[i];
//                     }
//                     else if(!freq2) {
//                         freq2 = KEY_FREQ[i];
//                     }
//                     else {
//                         break;
//                     }
//                     beat_drum(i, DURATION);
//                     audio_send_mix_wave(freq1, freq2, hit5.volume);
//                     break;
//                 }
//             }
        //}
        // Playback
//         if(toggle_play) {
//     		// cycle through the stored buffer and figure out how to deal with delays
//             while(!toggle_stop){
//                 main_cycle_sound();
//             }
//         }
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
	audio_send_tone(WAVE_SINE, play_out.drum, play_out.volume);
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