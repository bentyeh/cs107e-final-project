#ifndef SOUNDMAKER_H
#define SOUNDMAKER_H

#include "circular.h"

/*
Stores three integers:
* drum
* volume
* time_elapsed 
 */
//typedef volatile struct hit hit_t;

void soundmaker_init(void);
void soundmaker_record_beat(hit_t hit1);
volatile hit_t soundmaker_replay_beat();
int soundmaker_get_frequency(hit_t hit1);
int soundmaker_get_volume(hit_t hit1);
int soundmaker_get_delay(hit_t hit1);
void soundmaker_new_cir();
void soundmaker_clear_cir();

void soundmaker_vector(unsigned pc);

int get_d0_prev(void);
int get_d1_prev(void);
int get_d2_prev(void);
int get_d3_prev(void);

#endif
