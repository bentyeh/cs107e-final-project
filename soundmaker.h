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
void soundmaker_record_beat();
hit_t *soundmaker_replay_beat();
int soundmaker_get_frequency(int *hit1);
int soundmaker_get_volume(int *hit1);
int soundmaker_get_delay(int *hit1);


#endif