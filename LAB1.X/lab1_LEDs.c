#include <xc.h>

// The microcontroller "Hello World!"
void main (void)
{
    unsigned char i; //declare a variable
    // set the ports C and D as output
    
    TRISC = 0;
    TRISD = 0;
    
    while(1){
        /*Reset the LEDs*/
        PORTC = 0;
        PORTD = 0;
        /*Delay*/
        for(i = 0; i < 100; i++);
        /*Light LEDs*/
        PORTC=255;
        PORTD=255;
        //Delay
        for(i = 0 ; i < 100 ; i++);
    }
           
}

