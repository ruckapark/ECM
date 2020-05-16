#include <xc.h>

/************************************
/ Function delay
/ Used to introduce a delay of length t
 ************************************/
void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
    //Skeleton function for delay code
    //put code in here to make a delay
    //of length t
}

/************************************
/ Function LEDout
/ Used to display number on the LED array
/ in binary
 ************************************/
void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split

    //Skeleton function for displaying a binary number
    //on the LED array
    //you can write values to the whole port at once using by
    //changing LATN (e.g. LATC=something;)
    //or
    //individual pins can be changed using
    //LATNbits.LATNx (e.g. LATDbits.LATD2=1;)
}

void main(void) {
    int a = 1; //declare a as an integer
    int b = 0;
    //make a new variable that defines the direction that the lights go in

    LATC = 0; //set the output data latch levels to 0 on all pins
    LATD = 0;
    TRISC = 0; //set the data direction registers to output on all pins
    TRISD = 0;
    while (1) {
        while (b == 0) {
            a = a << 1; // increment a (shift the binary number to the left)
            if (a > 127) {
                b = 1;
            } //reset a when it gets too big
            LEDout(a); //output a on the LED array in binary
            // Delay so human eye can see change
            delay(100); //delay of length 100 millisecond
        }
        while (b == 1) {
            a = a >> 1;
            if (a < 2) {
                b = 0;
            } //go back to the first loop
            LEDout(a); //outputting again
            //delay
            delay(100);
        }
    }
}
