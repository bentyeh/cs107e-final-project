#include "gl.h"
#include "drumimage.h"
#include "malloc.h"
#include "timer.h"

static unsigned *drum_array[]; // array of x-values of centers of drums
static unsigned drum_diameter;
static unsigned num_drums;
const color RESTING_COLOR = GL_BLUE;
const color BEAT_COLOR = GL_RED;

void drumimage_init(unsigned gl_width, unsigned gl_height, unsigned num) {
    int i;

    gl_init(gl_width, gl_height, GL_DOUBLEBUFFER);
    num_drums = num;
    drum_diameter = min(gl_width / (2*num + 1), gl_height);
    
    drum_array = malloc(num_drums * sizeof(unsigned));
    for(i = 0; i<num_drums; i++) {
        drum_array[i] = (2*i+1) * gl_width / (2*num_drums + 1) + drum_diameter/2;
    }
    initialize_drums(num_drums);
}

void beat_drum(unsigned drum_num) {
    draw_drum(drum_num, BEAT_COLOR);
    delay_us(20000);
    draw_drum(drum_num, RESTING_COLOR);
}

void initialize_drums() {
    for(unsigned i = 0; i < num_drums; i++) {
        draw_drum(i, RESTING_COLOR);
    }
}

void draw_drum(unsigned drum_num, color c) {
    // gl_draw_circle(drum_array[drum_num], gl_height/2, c);
    gl_draw_rect(drum_array[drum_num] - drum_diameter/2, gl_height/2 - drum_diameter/2, drum_diameter, drum_diameter, c);
}