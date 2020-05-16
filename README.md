# ECM

This is a project carried out in the third year of imperial college. Uploaded for my own reference in C programming and for help to future ECM students.  
This project is a great way to learn procedural programming learning to code header structures, allocate memory and access it with pointers etc.


## Final Code  
Whilst I won't upload my full project, there are a couple of files for the reading of the IR beacons etc that may be useful if you get stuck on the project.

## LABS  

### lab1  
* config of pins etc.  
* Including functions required to use standard headers with microcontroller in C  

### lab2  
* LEDcounter - counter in decimal on LEDs  
* Button press - update decimal counter at the press of a button  
* Knight Rider - Counter that counts back down when reached limit  
* Analogue input - AD conversion with a volume sensore (volume output to LEDs)  

### lab3  
* ex1 - Switch counter direction by input signal - which introduces the high priority interrupt and the concept of debouncing a switch  
* ex3 - Introduce timers and time overflows (also handling with interrupts)  
		- part c is for 16 bit numbers and their handling (keeping 8 least important bits)  
		- part d incorporates the input from ex 1 in a low priority interrupt to combine interrupts  
* ex4 - Idea of a simpler switch debouncer checking for a constant value on voltage  
* miniproject - LDR use to implement street light algorithm to count the days turning on and off street lights appropriately using internal clock timer.  

### lab4 - using a screen for animations and visual output - ultimately using for robot messages  
* initialise - used for initialising all the pins in the screen to have it configured correctly.  
	- introduces the concept of defining in C (so we have out output and input pins defined with custom names  
	- uses inbuilt delay with internal clock  
	- delays are important for the LCD screen (length of time)  
	- using a 4 bit interface for the LCD screen  
	- all characters ASCII in a lookup table in LCD user manual  
	- introduces concept of a buffer  
* animation - uses previous lab in LDR to animate a rower going across screen at a speed dependant on the incoming light intensity

### lab5 - calibrating motors with PWM control - ultimately these were used on a cat track robot with seperate control for left and right side motors
* ex 1 - simple servomotor that can rotate 180 deg (used with PWM)
* ex 2 - turn servomotor 180 degrees when detect input pin change (for LDR for example)
* ex 3 - conversion of motor current to PWM setting up the motors etc.
* ex 4 - Important exercises for the left and right motors- using STRUCTURES AND POINTERS!!! Getting the motors working properly

- this will make the motor.h header file for the rest of the project, having various functions such as turn left, turn right, forward, stop, speed etc.
	
### lab6  
* ex 1a - sending voltages to an input pin and returning this value to the LCD display
* ex 1b - use of serial port - limited changes
* ex 2a - send various chars to the LCD
* ex 2b - Analogue to dig conversion with the output then sent automatically to LCD
* ex 3a - Reading RFID tag - always starts and ends with certain char so just run until start char is read - then stop upon end char
* ex 3b - Put RFID trigger into an interrupt of high priority!

## Headers  
Naturally for the final project almost all the functions were found elsewhere in header files.
I have included an example of a header for the PWM motor control.