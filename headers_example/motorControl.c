#include <xc.h>
#include "motorControl.h"

#define _XTAL_FREQ 8000000   //whats going on here

void initPWM(int ptper){
    
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
    /*bring motors to the same speed*/
    if((mL->power)<(mR->power)){
        while((mL->power)<(mR->power)){
            mL->power ++;
            setMotorPWM(mL);
            __delay_ms(1);
        }
    }else if((mR->power)<(mL->power)){
        while((mR->power)<(mL->power)){
            mR->power ++;
            setMotorPWM(mR);
            __delay_ms(1);
        }
    }
    
    //use similar to stop but in reverse where any lower than 25 is stationary anyway so bring up
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


