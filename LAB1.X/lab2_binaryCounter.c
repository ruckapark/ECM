#include <xc.h>
/************************************
/ Function delay
/ Used to introduce a delay of length t
************************************/
void delay (int t)
{
    int x;
    for(x=0; x<t; x++);
 //Skeleton function for delay code
 //put code in here to make a delay
 //of length t
}
/************************************
/ Function LEDout
/ Used to display number on the LED array
/ in binary
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
 //Skeleton function for displaying a binary number
 //on the LED array
 //you can write values to the whole port at once using by
 //changing LATN (e.g. LATC=something;)
 //or
 //individual pins can be changed using
 //LATNbits.LATNx (e.g. LATDbits.LATD2=1;)
}
void main (void)
{
 int a=0; //declare a as an integer

 LATC=0; //set the output data latch levels to 0 on all pins
 LATD=0;
 TRISC=0; //set the data direction registers to output on all pins
 TRISD=0;
 while (1){
 a++; // increment a
 if (a>255) {a=0;} //reset a when it gets too big
 LEDout(a); //output a on the LED array in binary
 // Delay so human eye can see change
 delay(100); //delay of length 100
 }
}
