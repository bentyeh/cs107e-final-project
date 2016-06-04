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


#endif

/*

#define DRUM_1 TOM_FREQ
#define DRUM_2 KICK_FREQ
#define DRUM_3 BONGO_FREQ
#define DRUM_4 CONGA_FREQ

int DRUM_FREQ[NUM_DRUMS] = {DRUM_1, DRUM_2, DRUM_3, DRUM_4};

*/