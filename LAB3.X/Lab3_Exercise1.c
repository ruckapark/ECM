#include <xc.h>

void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
}

void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split
}

// high priority interrupt
void interrupt InterruptHandlerHigh();

// global vars
int buttonPush = 0; 	//part a
int a = 0; 				//direction for part b
int bounceCounter = 0; 	//counter for switch debouncer

void main(void) {
    
    ///*Enable interrupt priority*/
    RCONbits.IPEN = 1;        //only if you actually want priority
    
    // Enable interrupts
    INTCONbits.GIEH = 1; // Global Interrupt Enable bit
    INTCONbits.GIEL = 1; // Global Interrupt Enable bit
    // Set the button on RC3 to trigger an
    // interrupt. It is always high priority
    INTCONbits.INT0IE = 1; //INT0 External Interrupt Enable bit
    
    LATC = 0;
    LATD = 0;
    TRISD = 0;
    TRISC = 0b00001000; //set pin c3 as input
    
    int counter = 1;
    
    while(1){
        while(bounceCounter < 25){bounceCounter++;}//debouncer for interrupt (otherwise may run straight back to interrupt!
        while (a == 0) { //count direction
            LEDout(counter);
            counter++;
            if(counter>127){a=1;}
            delay(25);
            
        }
        while(a == 1){ // a = 1 count up. a = 0 count down.
            LEDout(counter);
            counter--;
            if(counter<2){a=0;}
            delay(25);
            
        }
        
    }
    
    
    //to debounce the switch, use another counter i
    //if it i less that 20 dont bother interrupting
    
    if (PORTCbits.RC3 == 1){
        if (bounceCounter > 25) { //Avoids repeated interrupt
             INTCONbits.INT0IF = 1;
        } else {
            INTCONbits.INT0IF = 0; 
        }
    }
}

void interrupt InterruptHandlerHigh(){ //high priority routine

    if (INTCONbits.INT0IF)
    {
        //exercise a
//        buttonPush++; //increment a counter
//        LEDout(buttonPush);
//        INTCONbits.INT0IF = 0;  //clear the interrupt flag
//        delay(10);
        
        
        //exercise b
        if(a==0){a=1;} 			//if counting up change a down
        else {a=0;} 			//reverse if not
        bounceCounter = 0; 		//resets the counter for the next bounce
        INTCONbits.INT0IF = 0; 	//clear interrupt flag
    }
}