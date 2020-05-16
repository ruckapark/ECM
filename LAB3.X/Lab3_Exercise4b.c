#include <xc.h>
//set the timer in pragma config
#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

//include the LEDout function
void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split

}

void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
}

void interrupt InterruptHandlerHigh() { //high priority routine

    if (INTCONbits.INT0IF) {
        if (PORTCbits.RC3 == 1) {
            if (PORTCbits.RC3 == 1) { //these act as switch debouncer.
                if (PORTCbits.RC3 == 1) { //if it suddenly returns to zero interrupt cancel
                    //increment a counter
                    LEDout(TMR0L);
                }
            }
        }
    }
    INTCONbits.INT0IF = 0; //clear the interrupt flag

}//ask about what is going on here with the least significant bit

void main(void) {
    
    //setup the outputs
    LATC = 0;
    LATD = 0;
    TRISD = 0;
    TRISC = 0b00001000;     //set pin c3 as input

    // Generate an interrupt on timer overflow
    INTCONbits.GIEH = 1;    // Global Interrupt Enable bit
    INTCONbits.INT0IE = 1; //INT0 External Interrupt Enable bit
    
    //timer setup
    T0CONbits.TMR0ON = 1;       //turn on timer0
    T0CONbits.T016BIT = 1;      // 8bit mode
    T0CONbits.T0CS = 1;         // use internal clock (Fosc/4)
	//is this bit necessary or will all reg bits be 0 without call.
	
    while(1){}

}