# ajahanso-bentyeh-ericscrum-project
  
##Project Writeup

###About

We initially wanted to build a musical instrument that could capture volume input from the user, and generate different frequencies or notes along with a graphical representation indicating which drum has been hit. This would require libraries that combine sine wave frequencies, communicate with analog sensors, and generate a dynamic GUI, so we wrote our own math, audio, sensor, and graphics libraries with timer (and at one point GPIO) interrupts to fetch values from piezo disc sensors.

###Challenges

1. Piezo Elements Stick
To build our instrument, we used piezoelectric “knock” sensors. The sensors work by generating voltage when ceramic and electrode elements are energized (eg. by vibration or knock). When the two pieces fluctuate, a separation of charge is formed, generating a voltage (and subsequently a charge). The pros of using piezo knock elements was we could measure how hard a hit was based on the amount of voltage produced. The major drawback and flaw to the sensors was the charge generated may not be dissipated properly, and it collected inside the element. This meant the sensor would get “stuck” and output maximum voltage for an extended and inconsistent period of time. 
2. No SPI Interrupts
To read values from our analog sensors, we used a MCP3008 analog to digital converter to convert the sensor output to a digital value that the Pi could read. The chip uses an SPI protocol to communicate with the Pi. Because of the nature of the SPI protocol, there was no way for us to enable interrupts directly on the piezo sensors. Since SPI is contingent on the master __requesting__ information from the slave, it is not possible to enable an interrupt on a peripheral using SPI.
3. Multiple Frequencies at Once
If multiple sensors are triggered, then different frequencies should be accounted for in the sound that is played (the frequency of each note that the sensor is associated with). 
##Our Solutions
1. Timer Interrupts
We decided to use timer interrupts to sample all of the values from the sensors. This allowed us to constantly update our sensor readings.
2. [Dynamic Sound Wave Calculation](http://clas.mq.edu.au/speech/acoustics/waveforms/adding_waveforms.htm)
After obtaining the frequencies corresponding to the activated sensors and sensor output, we mixed the sound waves by calculating each mixture by adding the sine wave functions that corresponded to the activated sensors. We then adjusted the wave by scaling it with a base sine wave, and scaled the wave further with the volume given by the amount of voltage generated by the sensors. 
3. Sticky Piezo Discs
The piezo discs would get stuck at the maximum voltage because the charge that built up in the element could not be dissipated, resulting in “stuck” sensors. The only solution to this issue we could devise was to add a larger resistor to the sensors to dissipate some of the pent up charge. To achieve this, we “siphoned” voltage from the sensors by wiring them in parallel to the MCP3008 with a GPIO pin on the Pi.

###Switch in Focus
At this point, our instrument was not producing good tones consistently, so we decided to switch our focus to an alternative that could repurpose much of our code and sensor setup.  

We thought the sensitivity of the sensors would be an excellent model for detecting tremors in patients with Parkinson’s, tourette’s syndrome, or even pick up on the beginnings of an episode of epilepsy.  
We shifted from focusing on sound mixing and melodies (sad bc we already wrote functions to mix sine waves) and instead transitioned to focusing on converting the sensitivity in the sensors to a visual display that could conveniently show the size of the tremor.  We rewired five piezo sensors and attached them to a glove that would be worn by a prospective patient, and updated the graphics library to have a graph that displayed the vibration in each finger sensor.  If the tremor was violent enough to pass a threshold (specified by a #define and could be changed dependent on the disease targeted) the graphs would change from standard blue color to red.  
Then we added back in some of the audio functionality we had slaved over previously by playing a note if the threshold was surpassed, and this correlated to which finger triggered the influx.
Overall the idea is that one day there will be medical innovations capable of reading in the information from our sensors, and instead of outputting a graphical display and tones, they can be coupled to a device or catheter that can silence the nerves that are overacting and causing the tremor, relieving patients of symptoms. 

###Responsibilities Breakdown
*Eric* - Wrote the audio library, the sensor read functions, and constructed the physical mockup (wiring, breadboarding, soldering, etc.). Modified the PWM lecture code by adding wave mixing functions (in audio.c). Added functionality to the minimath library to write wave mixing functions (such as GCD calculations). Wrote a function/library on top of the cs107e SPI library to record the voltage of the piezo sensors. Finally, constructed the final mock-up of the project as well as the various different test setups. All code used was either from the cs107e.github.io repository or independently written for the project. The sensors were tested on an arduino before being used in the project, so the initial test code to evaluate the sensors was provided by the Arduino website (the piezo_chack.ino file in the repository).

*Alison* - Created a typedef struct hit_t which was used to store the sensor information after an interrupt occurred and rewrote the circular buffer program to be compatible with this new hit type.  Repurposed interrupts_handler from assignment 7 and responsible for implementing armtimer interrupts and gpio interrupts (gpio was not used in the long run).  Wrote soundmaker.c to handle the interrupts from the armtimer.  Only code borrowed was interrupts.c and armtimer.c from cs107e libpi.  

*Ben* -  Wrote the graphics, minimath library (which used a taylor series to calculate sine and cosine), and responsible for optimization of the interrupt handler as well as debugging wiring and main.  Created drumimage.c which modulated the graphics to match the senor output that was fetched from the interrupt handler, added playsounds() to main to scale the sounds.  Wrote all math functions himself without using any standard c libraries. 
