#include <xc.h>
//set the timer in pragma config
#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off


//include the LEDout function
void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split

}

//no need to define an integer here, we only have a timer overflow

int timerOverflows = 0; //

void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
}

void interrupt InterruptHandlerHigh()
{ //high priority routine

    if (INTCONbits.TMR0IF)
    {
        
        timerOverflows++; //increment a counter
        LEDout(timerOverflows);
        INTCONbits.TMR0IF = 0;  //clear the interrupt flag
    }

}

void main(void) {
    OSCCON = 0x72; //8MHz clock
    while (!OSCCONbits.IOFS); //wait for osc to become stable
    
    //setup the outputs
    LATC = 0;
    LATD = 0;
    TRISD = 0;
    TRISC = 0b00001000;     //set pin c3 as input

    
    // Generate an interrupt on timer overflow
    INTCONbits.GIEH = 1;    // Global Interrupt Enable bit
    INTCONbits.TMR0IE = 1;  //enable TMR0 overflow interrupt
    INTCON2bits.TMR0IP = 1; // TMR0 High priority
    
    //timer setup
    T0CONbits.TMR0ON = 1;       //turn on timer0
    T0CONbits.T016BIT = 0;      // 8bit mode
    T0CONbits.T0CS = 0;         // use internal clock (Fosc/4)
    T0CONbits.PSA = 1;          // disable prescaler
    T0CONbits.T0PS = 0b001;   // set prescaler value(not used)
    
    while(1){}

}
//write code for a counter that incremements when there is an overflow.
