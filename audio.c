
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#include "timer.h"
#include "audio.h"
#include "minimath.h"

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE         (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PWM_BASE          (BCM2708_PERI_BASE + 0x20C000) /* PWM controller */
#define CLOCK_BASE        (BCM2708_PERI_BASE + 0x101000)

// Crazy dwelch macro: compute the register as base + pin/10, offset as pin%10 * 3,
// the value (based on a) due to weird Pi numbering scheme
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define BCM2835_PWM_CONTROL 0
#define BCM2835_PWM_STATUS  1
#define BCM2835_PWM0_RANGE  4
#define BCM2835_PWM0_DATA   5
#define BCM2835_PWM_FIFO    6
#define BCM2835_PWM1_RANGE  8
#define BCM2835_PWM1_DATA   9

#define BCM2835_PWMCLK_CNTL   40
#define BCM2835_PWMCLK_DIV    41


#define BCM2835_PWM1_MS_MODE    0x8000  /*  Run in MS mode                   */
#define BCM2835_PWM1_USEFIFO    0x2000  /*  Data from FIFO                   */
#define BCM2835_PWM1_REVPOLAR   0x1000  /* Reverse polarity             */
#define BCM2835_PWM1_OFFSTATE   0x0800  /* Ouput Off state             */
#define BCM2835_PWM1_REPEATFF   0x0400  /* Repeat last value if FIFO empty   */
#define BCM2835_PWM1_SERIAL     0x0200  /* Run in serial mode             */
#define BCM2835_PWM1_ENABLE     0x0100  /* Channel Enable             */

#define BCM2835_PWM0_MS_MODE    0x0080  /* Run in MS mode             */
#define BCM2835_PWM0_USEFIFO    0x0020  /* Data from FIFO             */
#define BCM2835_PWM0_REVPOLAR   0x0010  /* Reverse polarity             */
#define BCM2835_PWM0_OFFSTATE   0x0008  /* Ouput Off state             */
#define BCM2835_PWM0_REPEATFF   0x0004  /* Repeat last value if FIFO empty   */
#define BCM2835_PWM0_SERIAL     0x0002  /* Run in serial mode             */
#define BCM2835_PWM0_ENABLE     0x0001  /* Channel Enable             */

#define BCM2835_BERR  0x100
#define BCM2835_GAPO4 0x80
#define BCM2835_GAPO3 0x40
#define BCM2835_GAPO2 0x20
#define BCM2835_GAPO1 0x10
#define BCM2835_RERR1 0x8
#define BCM2835_WERR1 0x4
#define BCM2835_EMPT1 0x2
#define BCM2835_FULL1 0x1

// Page 108 of the BCM2835 ARM Peripherals guide
#define BCM2835_CM_ENAB        (1 << 4)
#define BCM2835_CM_OSCILLATOR  (1)

#define PM_PASSWORD 0x5A000000 

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028


#define ERRORMASK (BCM2835_GAPO2 | BCM2835_GAPO1 | \
    BCM2835_RERR1 | BCM2835_WERR1)
#define MAXPRINT 5
volatile unsigned* gpio = (void*)GPIO_BASE;
volatile unsigned* clk  = (void*)CLOCK_BASE;
volatile unsigned* pwm  = (void*)PWM_BASE;


#define MAX_VOL_VAL 1024

unsigned waveform_square[64];
unsigned waveform_triangle[64];
unsigned waveform_saw[64];
// This is a sinusoid represented as 64 values in the range of [-32, 32]
unsigned waveform_sine[] = {32,
                       35,
                       38,
                       41,
                       44,
                       47,
                       49,
                       52,
                       54,
                       56,
                       58,
                       60,
                       61,
                       62,
                       63,
                       64,
                       64,
                       63,
                       63,
                       62,
                       61,
                       60,
                       58,
                       56,
                       54,
                       52,
                       49,
                       47,
                       44,
                       41,
                       38,
                       35,
                       32,
                       29,
                       26,
                       23,
                       20,
                       17,
                       15,
                       12,
                       10,
                       8,
                       6,
                       4,
                       3,
                       2,
                       1,
                       1,
                       0,
                       1,
                       1,
                       2,
                       3,
                       4,
                       6,
                       8,
                       10,
                       12,
                       15,
                       17,
                       20,
                       23,
                       26,
                       29,
                       32
};

/* This PWM module scales the 19.2MHz clock down to 8.192MHz.
   It respresents a signal as 8192 PWM pulses: 64 samples of
   128 pulses. So magnitude of a sample is the number of pulses
   (in the range of 0-128). */

/* There are four waves: square, triangle, saw, and sine. The first
   three are generated when the program starts, the last one is a
   lookup table compiled into the program. */


unsigned int audio_set_clock(unsigned int frequency) {
  frequency /= 2;
  frequency *= 8192;

  // You specify the PWM clock as a fraction of the 19.2MHz base clock.
  // The fraction is specified in two parts: the integer component
  // and a fractional component out of 1024. So, for example,
  // an 8192kHz (representing a 1kHz audio tone) clock is 2.34275;
  // this is stored as an integer part of
  // 2 and a fractional part of 352 (0.342 * 1024).
  unsigned int integer_part = 19200000 / frequency;
  unsigned int fractional_part = 19200000 % frequency;
  fractional_part *= 1024;
  fractional_part /= frequency;


  // Fractional part should always be less than 1024 (it's a fraction < 1
  // times 1024, but check to be sure).
  if (integer_part < (1 << 12) &&
      integer_part > 0 &&
      fractional_part < 1024) {
    // Stop the clock
    *(clk + BCM2835_PWMCLK_CNTL) = PM_PASSWORD | (1 << 5);
    // Reset clock
    *(clk + BCM2835_PWMCLK_DIV)  = PM_PASSWORD | (integer_part << 12) | fractional_part;
    return 1;
  } else {
    return 0;
  }
}

/* This function does not return. It transmits a over
   the RPi audio jack as a pulse-width-modulated signal. It sends a
   wave as 64 values of up to 128 pulses each (so the x-axis
   of the wave is quantized to 64 values and the Y axis is
   quantized to 128 values).

**For the RPi bongos, the waveform or wave type should always be sine

This is a generic send tone function that allows the caller to indicate
the wave shape, frequency, and volume (through modifying the waveform amplitude)
The tone is played for the length of a quarter note
*/


void audio_send_tone(wave_type_t type, unsigned int hz, int volume) {
  unsigned* waveform;
  if (type == WAVE_TRIANGLE) {
    waveform = waveform_triangle;
  } else if (type == WAVE_SINE) {
    waveform = waveform_sine;
  } else if (type == WAVE_SAW) {
    waveform = waveform_saw;
  } else {
    waveform = waveform_square;
  }
  
  if (audio_set_clock(hz)) {
    // Start the clock
    // enable (ENAB) + oscillator 
    // raspbian has this as plla
    *(clk + BCM2835_PWMCLK_CNTL) = PM_PASSWORD |
      BCM2835_CM_ENAB |
      BCM2835_CM_OSCILLATOR;
    
    delay_us(2000);
    
    // disable PWM
    *(pwm + BCM2835_PWM_CONTROL) = 0;
    
    delay_us(2000);
    
    // We are dividing each "step" of the sinusoid into 128
    // pulse slots.
    *(pwm+BCM2835_PWM0_RANGE) = 0x80;
    *(pwm+BCM2835_PWM1_RANGE) = 0x80;

    // Re-enable PWM
    *(pwm+BCM2835_PWM_CONTROL) =
      BCM2835_PWM1_USEFIFO | 
      BCM2835_PWM1_ENABLE | 
      BCM2835_PWM0_USEFIFO | 
      BCM2835_PWM0_ENABLE |
      1 << 6; // Clear the FIFO of any old data
    
    delay_us(2000);

    int i = 0;
    
    int tone_start = timer_get_time();
    while((timer_get_time() - tone_start) < (100000 / 4)) {
      int status =  *(pwm + BCM2835_PWM_STATUS);
      
      if (!(status & BCM2835_FULL1)) {
        *(pwm+BCM2835_PWM_FIFO) = ((waveform[i] * volume) / MAX_VOL_VAL);
        i++;
        i = i % 64;
      }
      if ((status & ERRORMASK)) {
        *(pwm+BCM2835_PWM_STATUS) = ERRORMASK;
      } 
    }
  }
}


void audio_init() {
  int i;
  SET_GPIO_ALT(40, 0);
  SET_GPIO_ALT(45, 0);
  delay_us(2000);
  for (i = 0; i < 64; i++) {
    waveform_square[i] = (i < 32)? 0: 64;
    waveform_triangle[i] = (i < 32)? (2 * i) : 64 - (2 * (i - 32));
    waveform_saw[i] = i;
  }
}

//test tone function
void audio_send_1kHz() {
  audio_send_tone(WAVE_SINE, 1000, 1024);
}

/* Sends the audio tone for a tom drum */
// void audio_send_tom(int vol){
// 	audio_send_tone(WAVE_SINE, TOM_FREQ, vol, 1000);
// }

/* Sends the audio tone for a cymbal */
// void audio_send_cymbal(int vol){
// 	audio_send_tone(WAVE_SINE, CYMBAL_FREQ, vol, 1000);
// }

/* Sends the audio tone for a kick drum */
// void audio_send_kick(int vol){
// 	audio_send_tone(WAVE_SINE, KICK_FREQ, vol, 1000);
// }

/*Sends the audio tone for a bongo drum */
// void audio_send_bongo(int vol){
// 	audio_send_tone(WAVE_SINE, BONGO_FREQ, vol, 1000);
// }

/* Sends the audio tone for a conga drum */
// void audio_send_conga(int vol){
// 	audio_send_tone(WAVE_SINE, CONGA_FREQ, vol, 1000);
// }

/* Sends the audio tone for a high-hat */
// void audio_send_high_hat(int vol){
// 	audio_send_tone(WAVE_SINE, HIGH_HAT_FREQ, vol, 1000);
// }

/* Helper for sending mixed audio waves */
static void audio_send_wave(unsigned wave, unsigned int hz, int volume) {
  unsigned* waveform; //will cause 'uninitialized warning -> ignore'
   waveform = wave;
  
  if (audio_set_clock(hz)) {
    // Start the clock
    // enable (ENAB) + oscillator 
    // raspbian has this as plla
    *(clk + BCM2835_PWMCLK_CNTL) = PM_PASSWORD |
      BCM2835_CM_ENAB |
      BCM2835_CM_OSCILLATOR;
    
    delay_us(2000);
    
    // disable PWM
    *(pwm + BCM2835_PWM_CONTROL) = 0;
    
    delay_us(2000);
    
    // We are dividing each "step" of the sinusoid into 128
    // pulse slots.
    *(pwm+BCM2835_PWM0_RANGE) = 0x80;
    *(pwm+BCM2835_PWM1_RANGE) = 0x80;

    // Re-enable PWM
    *(pwm+BCM2835_PWM_CONTROL) =
      BCM2835_PWM1_USEFIFO | 
      BCM2835_PWM1_ENABLE | 
      BCM2835_PWM0_USEFIFO | 
      BCM2835_PWM0_ENABLE |
      1 << 6; // Clear the FIFO of any old data
    
    delay_us(2000);

    int i = 0;
    
    int tone_start = timer_get_time();
    while((timer_get_time - tone_start) < (100000 / 4)) {
      int status =  *(pwm + BCM2835_PWM_STATUS);
      
      if (!(status & BCM2835_FULL1)) {
        *(pwm+BCM2835_PWM_FIFO) = ((waveform[i] * volume) / MAX_VOL_VAL);
        i++;
        i = i % 64;
      }
      if ((status & ERRORMASK)) {
        *(pwm+BCM2835_PWM_STATUS) = ERRORMASK;
      }
    }
  }
}


// Mixes two waves of different frequencies and sends the output to the audio jack
// The second frequency may be zero but not the first
int audio_send_mix_wave(int freq1, int freq2, int volume){
  //Pseudocode:
    //find greatest common denominator, set as wave frequency
    //calculate the sine waveform based on the hz for each frequency
    //add the wafeforms at each point (assuming in sync)
    //shift all points of the final waveform up by its lowest value
    //return 1 if successful, 0 otherwise

  if(freq1 == 0) return 0; //return error if the first frequency is 0
  //special case for if the second frequency is 0
  if(freq2== 0){
    audio_send_tone(WAVE_SINE, freq1, volume);
    return 1;
  }

  int wave_final[64];
  int freq = gcd(freq1, freq2);
  int adjuster = 64 / (2 * MATH_PI * 64);

  for(int i = 0; i < 64; i++){
    wave_final[i] = sin(freq1 * 2 * MATH_PI * adjuster) + sin(freq2 * 2 * MATH_PI * adjuster);
    wave_final[i] = wave_final[i] * waveform_sine[i]; //multiply by the base sine wave provided by Phil
    adjuster += adjuster;
  }
  //shift all values up so there are no negative values
  int min = min_arr(wave_final, 64);
  for(int i = 0; i < 64; i++){
    wave_final[i] += min;
  }

  audio_send_wave( (unsigned) wave_final, freq, volume);
  return 1;
}
