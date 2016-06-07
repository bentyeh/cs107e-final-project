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

#define SENSOR_THRESHOLD 500

/* Function prototypes */
int get_time_elapsed();
void set_buttons(int button);


/* Global variables */
cir_t* cir_record;
cir_t* cir_freeplay;
static int stored_time;
static int first_beat_time;
int value = 0;

static int num_keys;
static int *sensor_values;

extern int toggle_play; //can i get this from main?
extern int toggle_stop; //can i get this from main?
extern int ambient_vibration = 0; //global var to get the ambient viration of the room to calibrate

/*
 * initialize the sound maker
 */
void soundmaker_init(int keys) {

    // Set up GPIO pins for buttons
    set_buttons(START);
    set_buttons(STOP);
    set_buttons(PLAY);
    set_buttons(CLEAR);
    stored_time = 0;
    first_beat_time = 0;

    // Initialize buffers for freeplay and record
    cir_record = cir_new();
    cir_freeplay = cir_new();

    num_keys = keys;
    int temp[keys];
    sensor_values = malloc(sizeof(temp));

    // Sample the ambient environment for calibration
    ambient_vibration = sensors_get_ambient_vibration(3000);

    // Set up GPIO pin for sensor interrupts
    gpio_set_function(GPIO_INTERRUPT_PIN, GPIO_FUNC_INPUT);
    gpio_set_pulldown(GPIO_INTERRUPT_PIN);
}

/*
* This function is called every time a beat is hit and needs to be recorded
*** probably should be called by the interrupt handler
*/

void soundmaker_record_beat(hit_t hit1){
    // Add time_elapsed to the hit1 structure
    hit1.time_elapsed = get_time_elapsed();
    cir_enqueue(cir_record, hit1);
}

/*
* returns  the hit in the circular queue
*/

volatile hit_t soundmaker_replay_beat(){
    if(cir_empty(cir_record)){
        // return 0;
    }
    hit_t hit2 = cir_dequeue(cir_record);
    cir_enqueue(cir_record, hit2);
    return hit2;
}


/*
* Records the time elapsed between hits by storing the old time in a stored time 
* global and subtracting off the difference.
*/
int get_time_elapsed(){
    int new_time = timer_get_time();
    if(stored_time == 0){
        first_beat_time = new_time;
        return 0;
    }
    int elapsed_time = (new_time - stored_time);
    stored_time = new_time;
    return elapsed_time;
}


//getter functions for main
int soundmaker_get_drum(hit_t hit1){
    return hit1.drum;
}

//getter functions for main
int soundmaker_get_volume(hit_t hit1){
    return hit1.volume;
}

//getter functions for main
int soundmaker_get_delay(hit_t hit1){
    return hit1.time_elapsed;
}

    
/*This is a timer interrupt that fires to see if the drums were hit because
we cannot get interrupts directly from SPI*/

/* just play the note when we see an increase greater than 100 
Need to remember how mush it was the last time the interrupt checked */

void soundmaker_vector(unsigned pc){
    int i, sensor_mask, avg_volume, count;

    value = 0;

    //clear the interrupt
    if(!gpio_check_and_clear_event(GPIO_INTERRUPT_PIN)) {
        return;
    }
    // armtimer_clear_interrupt();
    
    //set initial values to zero because there might not have been any drum hit
    int sum = 0, drum = 0, num_drums = 0;

    sensor_mask = 0;
    count = 0;
    avg_volume = 0;
    for(i = 0; i < num_keys; i++) {
        sensor_values[i] = sensors_read_value(i);

        if(sensor_values[i] > SENSOR_THRESHOLD) {
            sensor_mask |= (1<<i);
            avg_volume += sensor_values[i];
            count++;
        }

        // Debugging - check how hard each sensor was hit
        printf("A2C %d value: %d\n", i, sensor_values[i]);
        printf("A2C %d adjusted: %d\n", i, sensor_values[i] - ambient_vibration); 
    }

    // Do nothing if no sensor values exceeded threshold
    if(!count) {
        return;
    }

    // Average volume of sensors whose value is above threshold
    avg_volume /= count;
    
    //generate a hit from the drums
    hit_t hit1;
    hit1.drum = sensor_mask;
    hit1.volume = avg_volume;
    hit1.time_elapsed = 0;
    
    //enqueue the hit into the freeplay circular buffer
    //The idea of this if condition is to evaluate if there was a hit, and not just bouncing
    // if((d0 > 800 && (d0 - d0_prev) > 10) || (d1 > 800 && (d1 - d1_prev) > 10) || 
    //     (d2 > 800 && (d2 - d2_prev) > 10) || (d3 > 800 && (d3 - d3_prev) > 10)){

    cir_enqueue(cir_freeplay, hit1);

    //enqueue in the recording function
    // if(!toggle_stop)
    //         soundmaker_record_beat(hit1);
}



void set_buttons(int button){
    gpio_set_function(button, GPIO_FUNC_INPUT); 
    gpio_set_pullup(button); 
    gpio_set_input(button);
}

void soundmaker_new_cir() {
    cir_record = cir_new();
}

void soundmaker_clear_cir() {
    cir_clear(cir_record);
}

