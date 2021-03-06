#include <xc.h>

#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

#define _XTAL_FREQ 8000000 //allows precise delay functions to work

/*Function intialise the PWM*/
void initPWM(){
    
    /*these shouldn't change as Tpwm is constant*/
    PTCON0 = 0b00001000;
    PTCON1 = 0b10000000;
    
    /**/
    PWMCON0 = 0b00101111;   //all independent mode
    PWMCON1 = 0;            //special features disabled

    int ptper = 0b100111000100; //time period value for timer to count to
    PTPERL = ptper & 0b11111111;
    PTPERH = (ptper & 0b111100000000) >> 8; //only keep important 6 bits

}
//
void angle2PWM(int ptmr){   //this fucntion is currently for a duty cycle length input - not angle
    
    /*if you want to convert from angles use the following formula*/
    //ptmr = 0.74*theta + 129
    
    /*0.5ms = -90, 2ms = 180*/
    //set 1ms for somewhere in between
    ptmr = ptmr<<2;
    
    PDC0L = ptmr & 0b11111111;
    PDC0H = (ptmr & 0b11111100000000) >> 8;
    
}


void main(void){
    
    OSCCON = 0x72;                  //8MHz Clock
    while (!OSCCONbits.IOFS);       //Wait for OSC to become stable
    
    
    /*Configure the pins necessary*/
    LATBbits.LATB0 = 0;
    TRISBbits.RB0 = 0;
    
    
    /* Init ADC */
    ANSEL0 = 0b00001000;
    ANSEL1 = 0;
    
    ADCON0 = 0b00001101;
    ADCON1 = 0b00000000;
    ADCON2 = 0b10101011;
    
    
    /*Configure the PWM registers*/
    initPWM();
    
    char startAnglePTMR = 62;       //-90 degrees
    int endAnglePTMR = 262;        //90 degrees
    int ADResult;                   //Light level         
    
    while (1) {
        
        ADCON0bits.GO = 1; //start conversion

        while (ADCON0bits.GO);  //finish conversion

        ADResult = ADRESL;
        ADResult += ((unsigned int)ADRESH << 8);

        if(ADResult > 800){angle2PWM(startAnglePTMR);}
        else{angle2PWM(endAnglePTMR);}

    };
}
