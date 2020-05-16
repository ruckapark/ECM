#include <xc.h>
#include <string.h>
#include <stdio.h>

#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

#define _XTAL_FREQ 8000000 //allows precise delay functions to work

#define ENABLE_BIT LATCbits.LATC0     //enable bit pin
#define DB7 LATDbits.LATD1            //define the send pins
#define DB6 LATDbits.LATD0
#define DB5 LATCbits.LATC2
#define DB4 LATCbits.LATC1
#define RS LATAbits.LATA6

#define DB7TRIS TRISDbits.RD1            //define the send pins
#define DB6TRIS TRISDbits.RD0
#define DB5TRIS TRISCbits.RC2
#define DB4TRIS TRISCbits.RC1

char messageStart;
int buf;

void interrupt InterruptHandlerHigh()
{
    if(PIR1bits.RCIF)
    {
        buf = getCharSerial();
        if(buf ==0x01){
        messageStart = 1;
        }
    
    }
}

int ptper = 200;

struct DC_motor {
        char power;
        char direction;
        unsigned char *dutyLowByte;
        unsigned char *dutyHighByte;
        char dir_pin;
        int PWMperiod;
    };
    
/*have a structure for the left and right motor*/
struct DC_motor motorL, motorR; 

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

void setMotorPWM(struct DC_motor *m)
{
    int PWMduty; //tmp variable to store PWM duty cycle

    PWMduty = (m->power*m->PWMperiod)/100;  //calculate duty cycle (value between 0 and PWMperiod)
    
    if (m->direction) //if forward direction
    {
        LATB=LATB|(1<<(m->dir_pin)); //set dir_pin high in LATB
		PWMduty=m->PWMperiod-PWMduty; //need to invert duty cycle as direction is high (100% power is a duty cycle of 0)
    }
    else //if reverse direction
    {
        LATB=LATB&(~(1<<(m->dir_pin))); //set dir_pin low in LATB
    }

	//write duty cycle value to appropriate registers
    *(m->dutyLowByte)=PWMduty<<2;
    *(m->dutyHighByte)=PWMduty>>6;
}

//increases a motor to full power over a period of time
void setMotorFullSpeed(struct DC_motor *m)
{
	for (m->power; (m->power)<=100; m->power++){ //increase motor power until 100
		setMotorPWM(m);	//pass pointer to m to setMotorSpeed function (not &m)
		__delay_ms(1);	//delay of 1 ms (100 ms from 0 to 100 full power)
	}
}

void fullSpeed(struct DC_motor *mL, struct DC_motor *mR){
    //use similar to stop but in reverse
    if((mL->power<25)||(mR->power<25)){
        mL->power = 25; //wheels have already stopped moving
        mR->power = 25; //as voltage is so low.
        setMotorPWM(mR);
        setMotorPWM(mL);
    }
    __delay_ms(1);
    
    while((mL->power < 100)||(mR->power < 100)){
        if(mL->power < 100){
            mL->power ++;
            setMotorPWM(mL);
        }
        if(mR->power < 100){
            mR->power ++;
            setMotorPWM(mR);
        }
        __delay_ms(1); //use same delay as given setFullSpeed function
    }
}

void stop(struct DC_motor *mL, struct DC_motor *mR){
    //reconfigure the structures to stop the robot
    
    //decrease the power to zero gradually
    while((mL->power > 25)||(mR->power > 25)){
        if(mL->power > 25){
            mL->power --;
            setMotorPWM(mL);
        }
        if(mR->power > 25){
            mR->power --;
            setMotorPWM(mR);
        }
        __delay_ms(1); //use same delay as given setFullSpeed function
    }
    
    mL->power = 0; //wheels have already stopped moving
    mR->power = 0; //as voltage is so low.
    setMotorPWM(mR);
    setMotorPWM(mL);
    __delay_ms(1);
    
    //at high direction pin - DC is maximum at ptper
    
}

void turnLeft(struct DC_motor *mL, struct DC_motor *mR){
    //use the stop function
    stop(mL,mR);
    
    //set right track direction forward and left back
    mR->direction = 1;
    mL->direction = 0;
    
    //either use setMotorFullSpeed or use fullSpeedAhead
    fullSpeed(mL,mR); 
}

void turnRight(struct DC_motor *mL, struct DC_motor *mR){
    //use the stop function
    stop(mL,mR);
    
    //set right track direction forward and left back
    mR->direction = 0;
    mL->direction = 1;
    
    //either use setMotorFullSpeed or use fullSpeedAhead
    fullSpeed(mL,mR); 
}

void forward(struct DC_motor *mL, struct DC_motor *mR){
    //ensure direction is forward for both of them
    mL->direction = 1;
    mR->direction = 1;
    //go back to full speed
    fullSpeed(mL,mR);
}

void reverse(struct DC_motor *mL, struct DC_motor *mR){
    //ensure direction is backwards
    mL->direction = 0;
    mR->direction = 0;
    //go back to full speed
    fullSpeed(mL,mR);
}

int getCharSerial(void){
    //while (!PIR1bits.RCIF);     //what for the receive interrupt to clear
    
    return RCREG;               //return byte in the RCREG
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
    
    /*reset the latches and configure the tristates*/
    LATC = 0;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 1;  //TRIS for the VGA port chip
    
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
    
    
    BAUDCONbits.BRG16 = 1; //set baud rate scaling to 16 bit mode
    /*set baud rate to be - 19200 - BRG should be 103*/
    SPBRG = 102; //low byte
    SPBRGH = 0;  //high byte
    
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continous receive mode
    RCSTAbits.SPEN = 1; //enable serial port, other settings default
    TXSTAbits.TXEN = 1; //enable transmitter, other settings default

    PIE1bits.RCIE = 1;  //enable interrupt
    
    while(1){
        //wont work because of the interrupt stuff!!
        
        if(messageStart == 1){
            buf = getCharSerial();
            while(buf != 0x04){      // Run this loop until the end byte 0x03 is read   
                if(buf == 0xF3){stop(&motorL,&motorR);}
                else if(buf == 0xF0){forward(&motorL,&motorR);}
                else if(buf == 0xF1){turnRight(&motorL,&motorR);}
                else if(buf == 0xF2){turnLeft(&motorL,&motorR);}
                buf = getCharSerial();
            } 
            
            
            messageStart != messageStart;
        }
//        getCharSerial();        
//        if(RCREG == 0x01){
//        /*trigger interrupt structure if necessary here*/
//        //LATCbits.LATC3 = 1;
//        getCharSerial();
//            while(RCREG != 0x04){      // Run this loop until the end byte 0x03 is read   
//                if(RCREG == 0xF3){stop(&motorL,&motorR);}
//                else if(RCREG == 0xF0){forward(&motorL,&motorR);}
//                else if(RCREG == 0xF1){turnRight(&motorL,&motorR);}
//                else if(RCREG == 0xF2){turnLeft(&motorL,&motorR);}
//                getCharSerial();
//            } 
//        }
        
        
        //currently inefficient because it will continue resetting the motor to go forward until changed
        //if(motorState == 0){stop(&motorL,&motorR);}
        //else if(motorState == 1){forward(&motorL,&motorR);}
        //else if(motorState == 2){turnRight(&motorL,&motorR);}
        //else if(motorState == 3){turnLeft(&motorL,&motorR);}
//        if(RCREG == 0xF0){forward(&motorL,&motorR);}
//        else if(RCREG == 0xF1){turnLeft(&motorL,&motorR);}
//        else if(RCREG == 0xF2){turnRight(&motorL,&motorR);}
//        else if(RCREG == 0xF3){stop(&motorL,&motorR);}
    }
    
    
    
}
