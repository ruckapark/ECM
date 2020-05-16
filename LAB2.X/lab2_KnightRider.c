#include <xc.h>

/************************************
/ Function delay
/ Delay of length t
 ************************************/
void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
}

/************************************
/ Function LEDout
/ Output num input in binary
 ************************************/
void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split
}

void main(void) {
    int a = 1;
    int direction = 0; //direction for lights

    LATC = 0; //output data latch levels to 0 on all pins
    LATD = 0;
    TRISC = 0; //set the data direction registers to output on all pins
    TRISD = 0;
    while (1) {
        while (direction == 0) {
            a = a << 1; // increment a (shift the binary number to the left)
            if (a > 127) {
                direction = 1;
            } //reset a when it gets too big
            LEDout(a); //output a on the LED array in binary
            // Delay so human eye can see change
            delay(100); //delay of length 100 millisecond
        }
        while (direction == 1) {
            a = a >> 1;
            if (a < 2) {
                direction = 0;
            } //go back to the first loop
            LEDout(a); //outputting again
            //delay
            delay(100);
        }
    }
}
