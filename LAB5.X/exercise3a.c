#include <xc.h>

#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

#define _XTAL_FREQ 8000000 //allows precise delay functions to work

/*Function intialise the PWM*/
void initPWM(){
    
    /*these shouldn't change as Tpwm is constant*/
    PTCON0 = 0b00000000;   //no prescaler
    PTCON1 = 0b10000000;
    
    /**/
    PWMCON0 = 0b01111111;   //enable 1-4 pwm
    PWMCON1 = 0;            //special features disabled

    int ptper = 200; //time period value for timer to count to for 10khz
    PTPERL = ptper & 0b11111111;
    PTPERH = ptper >> 8; //only keep important 6 bits

}

void DC2SpeedPWM(int ptmr){
    
    ptmr = ptmr<<2;
    
    PDC1L = ptmr & 0b11111111;
    PDC1H = (ptmr & 0b11111100000000) >> 8;
    PDC0L = ptmr & 0b11111111;
    PDC0H = (ptmr & 0b11111100000000) >> 8;
}

void main(void){
    
    OSCCON = 0x72;                  //8MHz Clock
    while (!OSCCONbits.IOFS);       //Wait for OSC to become stable
    
    
    /*Configure the pins necessary*/
    LATB = 0;
    TRISBbits.RB0 = 0;              //do this efficiently before
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    
    /*Configure the PWM registers*/
    initPWM();
    
    
    
    while (1) {

        DC2SpeedPWM(199);

    };
}