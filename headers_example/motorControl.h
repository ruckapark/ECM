/* 
 * File:   motorControl.h
 * Author: GR1514
 *
 * Created on 06 March 2018, 08:14
 */

#ifndef MOTORCONTROL_H
#define	MOTORCONTROL_H

#include <xc.h>
#define _XTAL_FREQ 8000000      //do you need to include this here?

struct DC_motor {
        char power;
        char direction;
        unsigned char *dutyLowByte;
        unsigned char *dutyHighByte;
        char dir_pin;
        int PWMperiod;
    };

void initPWM(int ptper);    //give a better name to ptper eg PWMper
void DCtoPWM(int ptmr);     //what does this one do?
void setMotorPWM(struct DC_motor *m);
void setMotorFullSpeed(struct DC_motor *m);
void fullSpeed(struct DC_motor *mL, struct DC_motor *mR);
void stop(struct DC_motor *mL, struct DC_motor *mR);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR);
void turnRight(struct DC_motor *mL, struct DC_motor *mR);
void forward(struct DC_motor *mL, struct DC_motor *mR);
void reverse(struct DC_motor *mL, struct DC_motor *mR);

#endif	/* MOTORCONTROL_H */

