#include "gl.h"
#include "drumimage.h"
#include "malloc.h"
#include "timer.h"
#include "minimath.h"

static unsigned *drum_array; // array of x-values of centers of drums
static unsigned drum_diameter;
static unsigned num_drums;
static unsigned gl_width;
static unsigned gl_height;
const color RESTING_COLOR = GL_BLUE;
const color BEAT_COLOR = GL_RED;

void drumimage_init(unsigned width, unsigned height, unsigned buffer, unsigned num) {
    unsigned i;

    gl_width = width;
    gl_height = height;
    num_drums = num;
    drum_diameter = min_u(gl_width / (2*num + 1), gl_height);
    drum_array = malloc(num_drums * sizeof(unsigned));

    gl_init(gl_width, gl_height, buffer);
    for(i = 0; i<num_drums; i++) {
        *(drum_array+i) = (2*i+1) * gl_width / (2*num_drums + 1) + drum_diameter/2;
    }
    gl_clear(GL_WHITE);
    initialize_drums(num_drums);
}

void beat_drum(unsigned drum_num) {
    draw_drum(drum_num, BEAT_COLOR);
    gl_swap_buffer();
    delay_us(20000);
    draw_drum(drum_num, RESTING_COLOR);
    gl_swap_buffer();
}

void initialize_drums() {
    for(unsigned i = 0; i < num_drums; i++) {
        draw_drum(i, RESTING_COLOR);
    }
}

void draw_drum(unsigned drum_num, color c) {
    // gl_draw_circle(drum_array[drum_num], gl_height/2, c);
    gl_draw_rect(*(drum_array + drum_num) - drum_diameter/2, gl_height/2 - drum_diameter/2, drum_diameter, drum_diameter, c);
}