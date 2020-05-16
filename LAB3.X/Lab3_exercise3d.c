#include <xc.h>
//set the timer in pragma config
#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

//include the LEDout function
void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split

}

//no need to define an integer here, we only have a timer overflow

int a = 0;
int timerOverflows = 10; //

void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
}

void interrupt InterruptHandlerHigh() { //high priority routine

    if (INTCONbits.INT0IF) {
        if (PORTCbits.RC3 == 1) {
            if (PORTCbits.RC3 == 1) { //these are like a switch debouncer.
                if (PORTCbits.RC3 == 1) { //if it suddenly returns to zero no interrupt
                    a=!a;
                }
            }
        }
        INTCONbits.INT0IF = 0; //clear the interrupt flag
    }
    //does it matter where i put that interrupt clear?

}

void interrupt low_priority InterruptHandlerLow() {
    if (INTCONbits.TMR0IF) {
        if (a == 0) {
            timerOverflows++; //increment a counter
            if (timerOverflows > 255) {
                a = !a;
            }
            LEDout(timerOverflows);
        } else {
            timerOverflows--;
            if (timerOverflows < 2) {
                a = !a;
            }
            LEDout(timerOverflows);
        }
    }
    INTCONbits.TMR0IF = 0; //clear the interrupt flag
}


void main(void) {
    OSCCON = 0x72; //8MHz clock
    while (!OSCCONbits.IOFS); //wait for osc to become stable
    
    //setup the outputs
    LATC = 0;
    LATD = 0;
    TRISD = 0;
    TRISC = 0b00001000;     //set pin c3 as input

    
    //timer setup
    T0CONbits.TMR0ON = 1;       //turn on timer0
    T0CONbits.T016BIT = 0;      // 16bit mode
    T0CONbits.T0CS = 0;         // use internal clock (Fosc/4)
    T0CONbits.PSA = 1;          // disable prescaler
    //T0CONbits.T0PS = 0b010;   // set prescaler value(not used)
    
    // Generate an interrupt on timer overflow
    RCONbits.IPEN = 1;      //enable priority
    INTCONbits.GIEH = 1;    // Enable High P bit
    INTCONbits.GIEL = 1;    // Global Interrupt Enable LP bit
    
    INTCONbits.TMR0IE = 1;  //enable TMR0 overflow interrupt
    INTCON2bits.TMR0IP = 0; // TMR0 Low priority
    INTCONbits.INT0IE = 1;  //enable external interrupt
    
    
    while(1){
        //LEDout(timerOverflows);
    }

}
//write code for a counter that incremements when there is an overflow.


//#include <xc.h>
//#pragma config OSC = IRCIO, WDTEN = OFF
//
//int a=10;
//int dir=0;
//
//void delay (int t)
//{
//    int i;
//    for (i=0; i<t; i++);
//}
//
//void LEDout(int number)
//{
//        LATC = (number&0b000111100) <<2 | LATC&0b000000011;
//        LATD = (number%4 <<2) | ((number&0b111000000) >>2) | LATC&0b000000011;
//}
//
//void interrupt InterruptHandlerHigh ()
//{
//    if (INTCONbits.INT0IF){ 
//        dir=!dir;               // increment a counter
//        
//        INTCONbits.INT0IF = 0;  //clear the interrupt flag
//    }
//}
//
//void interrupt low_priority InterruptHandlerLow ()
//{
//    if (INTCONbits.TMR0IF) { 
//        if(dir==0){
//            a++;
//        } else {
//            a--; 
//        }            
//     
//        INTCONbits.TMR0IF = 0; //clear the interrupt flag
//        TMR0H = 3000>>8;     
//        TMR0L = 3000; 
//    }
//}
//
//void main (void)
//{
//    OSCCON = 0x72; //8MHz clock ? why 0x72? Check the datasheet!
//    while(!OSCCONbits.IOFS); //Wait for OSC to become stable 
//    
//    LATC=0;             //set the output data latch levels to 0 on all pins
//    LATD=0;
//    TRISC=0b00001000;   //set the data direction registers to output on all pins
//    TRISD=0;
//    
//    T0CONbits.TMR0ON=1;   //turn on timer0
//    T0CONbits.T016BIT=0;  // 16bit mode
//    T0CONbits.T0CS=0;     // use internal clock (Fosc/4)
//    T0CONbits.PSA=0;      // enable prescaler
//    T0CONbits.T0PS=0b100; // set prescaler value 1:32
//    TMR0H = 3000>>8;     
//    TMR0L = 3000;        
//    
//    RCONbits.IPEN = 1;                                 //enables priority levels on interrupts
//    INTCONbits.GIEH = 1;     //global interrupt enabler
//    INTCONbits.GIEL = 1;     //enables all low-priority interrupts
//    
//    INTCONbits.TMR0IE = 1;   //enable timer0 overflow interrupt
//    INTCON2bits.TMR0IP = 0;  //set to low priority
//    INTCONbits.INT0IE = 1;   //RC3 interrupt enabler (always high priority)
//    
//    while (1){ 
//        LEDout(a);
//    }
//}