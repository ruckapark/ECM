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
    //c = LATC; //read all the pins in REG C
    //d = LATD; //dame for D
    
    //c= c & 0b00001111; //only keep the pins we aren't changing
    //d= d & 0b11000011;
    
    LATC = ((number & 0b00111100)<<2)|(LATC & 0b00001111); //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000)>>2)|((number & 0b00000011)<<2)|(LATD & 0b11000011); //just combine the part of the d reg split
    
    
    
    
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

