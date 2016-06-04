#include "drumimage.h"
#include "malloc.h"
#include "timer.h"
#include "minimath.h"

static unsigned *drum_array; // array of x-values of centers of drums
static unsigned drum_diameter;
static unsigned num_drums;
static unsigned gl_width;
static unsigned gl_height;
const color BACKGROUND_COLOR = GL_WHITE;
const color RESTING_COLOR = GL_BLUE;
const color BEAT_COLOR = GL_RED;

void drumimage_init(unsigned width, unsigned height, unsigned num) {
    unsigned i;

    // Stores initialization arguments into global variables
    gl_width = width;
    gl_height = height;
    num_drums = num;

    // Calculate diameter for a drum
    drum_diameter = min_u(gl_width / (2*num + 1), gl_height);

    // Malloc memory for drum_array
    drum_array = malloc(num_drums * sizeof(unsigned));

    // Set values in drum_array
    for(i = 0; i<num_drums; i++) {
        *(drum_array+i) = (2*i+1) * gl_width / (2*num_drums + 1) + drum_diameter/2;
    }

    // Initialize graphics library with single buffer
    gl_init(gl_width, gl_height, GL_SINGLEBUFFER);

    // Clear the screen to the background color
    gl_clear(BACKGROUND_COLOR);

    // Initialize the drums (draw drums with RESTING_COLOR)
    initialize_drums(num_drums);
}

void beat_drum(unsigned drum_num, unsigned beat_duration) {
    draw_drum(drum_num, BEAT_COLOR);
    delay_us(beat_duration);
    draw_drum(drum_num, RESTING_COLOR);
}

/*void beat_drum(unsigned drum_mask, unsigned beat_duration) {
    for(int i = 0; i < NUM_DRUMS; i++) {
        if(hit1.drum & (1<<i)) {
            draw_drum(i, BEAT_COLOR);
        }
    }
    delay_us(beat_duration);
    initialize_drums;
}*/

void initialize_drums() {
    for(unsigned i = 0; i < num_drums; i++) {
        draw_drum(i, RESTING_COLOR);
    }
}

void draw_drum(unsigned drum_num, color c) {
    // gl_draw_circle(drum_array[drum_num], gl_height/2, c);
    gl_draw_rect(*(drum_array + drum_num) - drum_diameter/2, gl_height/2 - drum_diameter/2, drum_diameter, drum_diameter, c);
}