#include <xc.h>
/************************************
/ Function delay
/ Return delay of length t
************************************/
void delay (int t)
{
    int x;
    for(x=0; x<t; x++);
}

/************************************
/ Function LEDout
/ Display number to the LED array in binary
/ And logic sends 8 bit number to displays
************************************/
void LEDout(int number)
{   
    LATDbits.LATD6 = number & 0b00000001;
    LATDbits.LATD5 = number & 0b00000010;
    LATDbits.LATD4 = number & 0b00000100;
    LATCbits.LATC7 = number & 0b00001000;
    LATCbits.LATC6 = number & 0b00010000;
    LATCbits.LATC5 = number & 0b00100000;
    LATCbits.LATC4 = number & 0b01000000;
    LATDbits.LATD3 = number & 0b10000000;
}

/*Count with LEDs from 0 to 255 with binary*/
void main (void)
{
 int a=0; //counter
 
 //set output latch 0 on all CD pins
 LATC=0;
 LATD=0;
 //set data direction reg to output 0 on all pins
 TRISC=0;
 TRISD=0;
 
 while (1){
 a++;
 if (a>255) {a=0;} 	//repeat counter
 LEDout(a); 		//output to LED counter
 delay(100);
 }
}
