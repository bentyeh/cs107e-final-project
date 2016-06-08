#include "soundmaker.h"
#include "armtimer.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "sensors.h" 
#include "tone.h"
#include "audio.h"
#include "printf.h"
#include "malloc.h"


#define START   GPIO_PIN20
#define STOP    GPIO_PIN21
#define PLAY    GPIO_PIN22
#define CLEAR   GPIO_PIN23

#define GPIO_INTERRUPT_PIN  GPIO_PIN19

#define SENSOR_THRESHOLD 0

/* Function prototypes */
int get_time_elapsed();
void set_buttons(int button);


/* Global variables */
cir_t* cir_record;
cir_t* cir_freeplay;
static int stored_time;
static int first_beat_time;
int value = 0;

static hit_t oldhit;

static int num_keys;
// int *sensor_values;

extern int toggle_play; //can i get this from main?
extern int toggle_stop; //can i get this from main?
extern int ambient_vibration = 0; //global var to get the ambient viration of the room to calibrate


/*
 * initialize the sound maker
 */
void soundmaker_init(int keys) {

    // Initialize buffers for freeplay and record
    cir_record = cir_new();
    cir_freeplay = cir_new();

    num_keys = keys;
    int temp[keys];
    // sensor_values = malloc(sizeof(temp));

    // Sample the ambient environment for calibration
    ambient_vibration = sensors_get_ambient_vibration(3000);

    // Set up GPIO pin for sensor interrupts
    gpio_set_function(GPIO_INTERRUPT_PIN, GPIO_FUNC_INPUT);
    gpio_detect_rising_edge(GPIO_INTERRUPT_PIN);
    gpio_set_pulldown(GPIO_INTERRUPT_PIN);
}

/*
* This function is called every time a beat is hit and needs to be recorded
*** probably should be called by the interrupt handler
*/

void soundmaker_record_beat(hit_t hit1){
//     Add time_elapsed to the hit1 structure
//     hit1.time_elapsed = get_time_elapsed();
//     cir_enqueue(cir_record, hit1);
}

/*
* returns  the hit in the circular queue
*/

volatile hit_t soundmaker_replay_beat(){
//     if(cir_empty(cir_record)){
//         // return 0;
//     }
//     hit_t hit2 = cir_dequeue(cir_record);
//     cir_enqueue(cir_record, hit2);
//     return hit2;
}


/*
* Records the time elapsed between hits by storing the old time in a stored time 
* global and subtracting off the difference.
*/
int get_time_elapsed() {
//     int new_time = timer_get_time();
//     if(stored_time == 0){
//         first_beat_time = new_time;
//         return 0;
//     }
//     int elapsed_time = (new_time - stored_time);
//     stored_time = new_time;
//     return elapsed_time;
}

void soundmaker_vector(unsigned pc) {
	//clear the interrupt
	armtimer_clear_interrupt();

	hit_t hit1;
	int do_enqueue = 0;

	for(int i = 0; i < num_keys; i++) {
		hit1.value_array[i] = sensors_read_value(i);
		if(!do_enqueue && hit1.value_array[i] != oldhit.value_array[i]) {
			do_enqueue = 1;
		}
	}
	
	if(do_enqueue) {
		cir_enqueue(cir_freeplay, hit1);
		oldhit = hit1;
	}
}