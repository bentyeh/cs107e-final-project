#ifndef SOUNDMAKER_H
#define SOUNDMAKER_H


/*
Stores three integers:
* frequency
* volume
* time_elapsed 
 */
typedef volatile struct hit hit_t;

void soundmaker_init(void);
void soundmaker_record_beat(int drum, int i);
volatile hit_t soundmaker_replay_beat();
int soundmaker_get_frequency(hit_t hit1);
int soundmaker_get_volume(hit_t hit1);
int soundmaker_get_delay(hit_t hit1);
void soundmaker_new_cir();
void soundmaker_clear_cir();


#endif