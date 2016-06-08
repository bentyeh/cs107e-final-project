#ifndef GL_GRAPH_H
#define GL_GRAPH_H

#include "gl.h"
#include "circular.h"

void graph_init(unsigned width, unsigned height, unsigned num_vars, unsigned threshold);
void graph_values(int num_values, hit_t hit1, unsigned threshold);
void draw_threshold_line(unsigned threshold);

#endif