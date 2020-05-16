#include <xc.h>

#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

#define _XTAL_FREQ 8000000 //allows precise delay functions to work

int ptper = 200;

/*Function intialise the PWM*/
void initPWM(){
    
    /*these shouldn't change as Tpwm is constant*/
    PTCON0 = 0b00000000;   //no prescaler
    PTCON1 = 0b10000000;
    
    /**/
    PWMCON0 = 0b01111111;   //enable 1-4 pwm
    PWMCON1 = 0;            //special features disabled

     //time period value for timer to count to for 10khz
    PTPERL = ptper & 0b11111111;
    PTPERH = ptper >> 8; //only keep important 6 bits

}

void DCtoPWM(int ptmr){
    
    ptmr = ptmr<<2;
    
    PDC1L = ptmr & 0b11111111;
    PDC1H = (ptmr & 0b11111100000000) >> 8;
    PDC0L = ptmr & 0b11111111;
    PDC0H = (ptmr & 0b11111100000000) >> 8;
}

void setMotorPWM(int speed, char direction, char motor){
    
    speed = speed << 2;   //PDC registers least sig 2 bits are left as empty
    
    if(motor == 1){
        if(direction == 1){
            LATBbits.LATB2 = 1;
            
            PDC1L = speed & 0b11111111;
            PDC1H = (speed & 0b11111100000000) >> 8;
        }else{
            speed = speed >>2;
            speed = ptper-speed;
            speed = speed <<2;
            
            LATBbits.LATB2 = 0;
            
            PDC1L = speed & 0b11111111;
            PDC1H = (speed & 0b11111100000000) >> 8;
        } 
        
    }else{
        if(direction == 1){
            LATBbits.LATB0 = 1;
            
            PDC0L = speed & 0b11111111;
            PDC0H = (speed & 0b11111100000000) >> 8;
        }else{
            LATBbits.LATB0 = 0;
            
            speed = speed >>2;
            speed = ptper-speed;
            speed = speed <<2;
            
            PDC0L = speed & 0b11111111;
            PDC0H = (speed & 0b11111100000000) >> 8;
        }
    }
    
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
    
    /*just return these values for now - use pointers later*/
    //int speed;                  //anywhere from 0-199
    //char direction;             //1 forwards  0 backwards
    //char motor;                 //1 pwm 3 0 pwm 1
    
    //use a function with pointers to change these values
    
    setMotorPWM(100,1,1);  //currently
    setMotorPWM(100,0,0);
    
    while (1){};
}
