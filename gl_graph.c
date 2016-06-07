#include "gl_graph.h"
#include "malloc.h"
#include "timer.h"
#include "minimath.h"
#include "printf.h"

#define HEIGHT_RESOLUTION 100
#define THRESHOLD_VALUE 50

static unsigned *bar_x_pos_array;
static unsigned num_bars;
static unsigned bar_width;
static unsigned gl_width;
static unsigned gl_height;
static unsigned y_tick;
const color BACKGROUND_COLOR = GL_WHITE;
const color NORMAL_COLOR = GL_BLUE;
const color THRESHOLD_COLOR = GL_RED;

void graph_init(unsigned width, unsigned height, unsigned num_vars) {

    // Stores initialization arguments into global variables
    gl_width = width;
    gl_height = height;
    num_bars = num_vars;

    // Calculate width of each bar
    bar_width = gl_width/(2*num_bars);
    bar_x_pos_array = (unsigned *)malloc(num_bars * sizeof(unsigned));
    for(int i = 0; i < num_bars; i++) {
        bar_x_pos_array[i] = 2*i*bar_width;
        printf("pos: %d\n", bar_x_pos_array[i]);
    }

    // Calculate height of each y-value
    y_tick = height / HEIGHT_RESOLUTION;

    // Initialize graphics library with single buffer
    gl_init(gl_width, gl_height, GL_DOUBLEBUFFER);

    // Clear the screen to the background color
    gl_clear(BACKGROUND_COLOR);
    gl_swap_buffer();
    gl_clear(BACKGROUND_COLOR);
}

void graph_values(int num_values, hit_t hit1) {
    int i, tmp;
    for(i = 0; i < num_values; i++) {
        tmp = hit1.value_array[i];
        if(tmp > THRESHOLD_VALUE) {
            gl_draw_rect(bar_x_pos_array[i], 0, bar_width, y_tick*hit1.value_array[i], THRESHOLD_COLOR);
        }
        else {
            gl_draw_rect(bar_x_pos_array[i], 0, bar_width, y_tick*hit1.value_array[i], NORMAL_COLOR);
        }
        printf("sensor %d: %d  ", i, hit1.value_array[i]);
    }
    printf("\n");
    gl_swap_buffer();
    gl_clear(BACKGROUND_COLOR);
}