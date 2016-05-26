# ajahanso-bentyeh-ericscrum-project
  
Fundamentally we need to figure out how we are going to be parsing the information 
from the sensor input, and implement something similar to a keyboard "read scancode"

Interrupts used to read the sensors:

Step 1:

	 -now we will have the tone (from the specific sensor) and the force of the hit (which
	 -we can translate to loudness) 
	 
	 -we can enqueue and dequeue the sound using a circular queue
	 
	 -we will use interrupts for handling the sensors, and a circular queue to store the 
	 sounds
	 
Step 2:

	-setting a beat
	
	-2 buttons:
		-press button A and then play for up to 10 seconds
		-press button A again to store beat
		
		-press button B to replay the stored beat
		
		-if you already have a beat stored, then when you press A it will be overwitten by 
		the new tone
		
	-In order to keep track of the beat:
		-creat a struct with 
			-volume
			-frequency
			-time elapsed since the last 

	  
