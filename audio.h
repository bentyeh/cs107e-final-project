#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

/* Hardware abstraction functions for bare metal pulse-width
 * modulation (PWM) audio on the Raspberry Pi.
 *
 * NOTE: To use this library, you must first call timer_init().
 * Audio initialization requires timers for busy waiting.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: September 10 2014
 */ 

/* Defines for the frequencies of different drums */
#define TOM_FREQ 120 //full
#define CYMBAL_FREQ 200 //clank
#define KICK_FREQ 60 //thump
#define BONGO_FREQ 80 //approx
#define CONGA_FREQ 100 //approx
#define HIGH_HAT_FREQ 10 //sizzle

typedef enum {
  WAVE_SQUARE,
  WAVE_SINE,
  WAVE_SAW,
  WAVE_TRIANGLE
} wave_type_t;

void audio_init();
void audio_send_1kHz();
void audio_send_tone(wave_type_t type, unsigned int hz, int volume);
unsigned int audio_max_tone_hz();
unsigned int audio_min_tone_hz();

/* Drum specific functions */
// void audio_send_tom(int vol);
// void audio_send_cymbal(int vol);
// void audio_send_kick(int vol);
// void audio_send_bongo(int vol);
// void audio_send_conga(int vol);
// void audio_send_high_hat(int vol);

int audio_send_mix_wave(int freq1, int freq2, int volume);

#endif

/*
 * Copyright (c) 2014 Stanford University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Stanford University nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STANFORD
 * UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
