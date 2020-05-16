#include <xc.h>
#include "motorControl.h"

#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

#define _XTAL_FREQ 8000000 //allows precise delay functions to work

int ptper = 200;
    
/*have a structure for the left and right motor*/
struct DC_motor motorL, motorR; //no need to be global

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
    
    motorL.power=0;                                 //zero power to start
    motorL.direction=1;                             //set default motor direction
    motorL.dutyLowByte=(unsigned char *)(&PDC1L);   //store address of PWM duty low byte
    motorL.dutyHighByte=(unsigned char *)(&PDC1H);  //store address of PWM duty high byte
    motorL.dir_pin=2;       //b2                        //pin RB0/PWM0 controls direction
    motorL.PWMperiod=ptper;                         //store ptper for motor - global variable here
    
    motorR.power=0;                                 //zero power to start
    motorR.direction=1;                             //set default motor direction
    motorR.dutyLowByte=(unsigned char *)(&PDC0L);   //store address of PWM duty low byte
    motorR.dutyHighByte=(unsigned char *)(&PDC0H);  //store address of PWM duty high byte
    motorR.dir_pin=0;       //b0                        //pin RB0/PWM0 controls direction
    motorR.PWMperiod=ptper;                         //store ptper for motor - global variable here

    
    /*just return these values for now - use pointers later*/
    //int speed;                  //anywhere from 0-199
    //char direction;             //1 forwards  0 backwards
    //char motor;                 //1 pwm 3 0 pwm 1
    
    //use a function with pointers to change these values
    
    setMotorPWM(&motorL);  //currently
    setMotorPWM(&motorR);
    
    while (1){
        forward(&motorL,&motorR);
        for (int i = 0; i < 20; i++){__delay_ms(50);}
        turnLeft(&motorL,&motorR);
        for (int i = 0; i < 20; i++){__delay_ms(50);}   //could make this into its own function but seems unnecessary
        turnRight(&motorL,&motorR);
        for (int i = 0; i < 20; i++){__delay_ms(50);}
        stop(&motorL,&motorR);
        for (int i = 0; i < 20; i++){__delay_ms(50);}
    };
}
