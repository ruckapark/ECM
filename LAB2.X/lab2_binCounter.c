#include <xc.h>
/************************************
/ Function delay
************************************/
void delay (int t)
{
    int x;
    for(x=0; x<t; x++);
}

/************************************
/ Function LEDout
/ Output num input in binary
 ************************************/
void LEDout(int number)
{   
    //c = LATC; //read all the pins in REG C
    //d = LATD; //same for D
    //c= c & 0b00001111; //shift into middle between d reg
    //d= d & 0b11000011;
    
    LATC = ((number & 0b00111100)<<2)|(LATC & 0b00001111); //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000)>>2)|((number & 0b00000011)<<2)|(LATD & 0b11000011); //just combine the part of the d reg split
}

void main (void)
{
 int a=0;

 LATC=0; 	//set the output data latch levels to 0 on all pins
 LATD=0;
 TRISC=0; 	//set the data direction registers to output on all pins
 TRISD=0;
 while (1){
 a++;
 if (a>255) {a=0;} 	//reset a when it exceded 8bit
 LEDout(a); 		//output binary
 delay(100); 		//delay for visual purpose
 }
}

