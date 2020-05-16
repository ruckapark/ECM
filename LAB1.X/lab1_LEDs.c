#include <xc.h>

// Control LEDs on microcontroller
void main (void)
{
    unsigned char i; //declare a variable
	
    // set port C and D as outputs
    TRISC = 0;
    TRISD = 0;
    
    while(1){
        //Reset the LEDs
        PORTC = 0;
        PORTD = 0;
		
        //Delay
        for(i = 0; i < 100; i++);
		
        //Light LEDs
        PORTC=255;
        PORTD=255;
		
        //Delay
        for(i = 0 ; i < 100 ; i++);
    }
           
}

