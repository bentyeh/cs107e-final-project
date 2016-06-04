#ifndef DRUMIMAGE_H
#define DRUMIMAGE_H

#include "gl.h"

void drumimage_init(unsigned gl_width, unsigned gl_height, unsigned num);

// Flashes the color of a drum for a specified duration
void beat_drum(unsigned drum_num, unsigned beat_duration);

// Draws all drums in their resting color
void initialize_drums();

// Draw a drum with a specified color
void draw_drum(unsigned drum_num, color c);

#endif