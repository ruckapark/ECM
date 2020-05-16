#include <xc.h>
//set the timer in pragma config
#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

int ADResult;
int ADResultOld;
int counter = 0;
int dayCounter = 0;
int totalDayLength = 120;  //based on processor speed just over 4 minutes
int nightLength;
int midnight;


void LEDout(int number) {
    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split
}

void LEDout10(int number)
{
    LATC = ((number & 0b00111100)<<2)|(LATC & 0b00001111);
    LATD = (((number & 0b1111000000)>>2)|((number & 0b00000011)<<2))|(LATD & 0b00000011);
}

//for daylight savings condition at beginning checking days counter
void interrupt InterruptHandlerHigh() { //high priority routine

    if (INTCONbits.TMR0IF) {

        ADCON0bits.GO = 1; //start conversion

        while (ADCON0bits.GO); //finish conversion

        ADResult = ADRESL;
        ADResult += ((unsigned int) ADRESH << 8);

        
        if ((ADResult<800)&(ADResult>600)){     //in practise could have a more precise switching value
            if(ADResultOld>ADResult){           //night to day
                counter = 0;                    //reset counter
                dayCounter++;                   //new day - could add condition for day counter = 365
                LEDout10(0);                    //lights off
            }
            else{
                nightLength = (totalDayLength-counter);     //calculate the lenght of the night
                counter = 0;                                //reset the time to count to 1am
                midnight = nightLength/2;                   //when is midnight? division is long
                LEDout10(0b1111111111);                     //night time starts
            }
        } else if (ADResult <601){
            counter++;              //increment counter
            LEDout10(0);            //lights off
        } else{
            if((counter>(midnight+4))&(counter<(midnight+26))){
                LEDout10(0);
                counter++;
            }else{
                counter++;                
                LEDout10(0b1111111111);   //lights on
            }
        }
        
        ADResultOld = ADResult; //store the old light value
        INTCONbits.TMR0IF = 0; //reset flag
        //        timerOverflows++; //increment a counter
        //        LEDout(timerOverflows);
        //        INTCONbits.TMR0IF = 0;  //clear the interrupt flag
    }

}


void main(void) {
    OSCCON = 0x72; //8MHz clock
    while (!OSCCONbits.IOFS); //wait for osc to become stable
    
    //setup the outputs
    LATC = 0;
    LATD = 0;
    TRISA = 0b00001000;     //LDR input
    TRISD = 0;
    TRISC = 0b00001000;     //set pin c3 as input

    ANSEL0 = 0b00001000;  //an3 (ldr pin) input
    ANSEL1 = 0;             //rest of an pins are output
    
    /* Init ADC */
    ADCON0 = 0b00001101;
    ADCON1 = 0b00000000;
    ADCON2 = 0b10101011; //if we care about conversion speed, try to understand REG
    
    
    //timer setup
    T0CONbits.TMR0ON = 1;       //turn on timer0
    T0CONbits.T016BIT = 0;      // 16bit mode
    T0CONbits.T0CS = 0;         // use internal clock (Fosc/4)
    T0CONbits.PSA = 0;          // disable prescaler
    T0CONbits.T0PS = 0b101;   // set prescaler value(not used)
    
    // Generate an interrupt on timer overflow
    //RCONbits.IPEN = 1;      //enable priority
    INTCONbits.GIEH = 1;    // Enable High P bit
    //INTCONbits.GIEL = 1;    // Global Interrupt Enable LP bit
    
    INTCONbits.TMR0IE = 1;  //enable TMR0 overflow interrupt
    INTCON2bits.TMR0IP = 1; // TMR0 priority
    //INTCONbits.INT0IE = 1;  //enable external interrupt
    
    
    while(1){
        //LEDout(timerOverflows);
    }

}
