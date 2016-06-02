#ifndef DRUMIMAGE_H
#define DRUMIMAGE_H

void drumimage_init(unsigned gl_width, unsigned gl_height, unsigned num);

void beat_drum(unsigned drum_num);

void initialize_drums();

void draw_drum(unsigned drum_num, color c);

#endif