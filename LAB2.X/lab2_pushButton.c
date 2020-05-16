#include <xc.h>

/*
/TRISC = 0b00001000|TRISC;
/TRISCbits.RC3 = 1; 
/both set RC3 as inp but only line 3 ensures others are outputs
*/

/*
/two options to read input on button press: 
/A = PORTCbitsRC3;
/or we could use A = PORTC & 0b00001000;
*/

// C reg represents bit 2-6 D reg represents bit 0-2 and 6-8
void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split
}


/* 
/ Counter from 0 - 100 returning the binary counter value in decimal:
/ 1 = 1 - 11 = 2 - 111 = 3 - 1111 = 4
/ 35 -> 3.5 tens -> 3 tens -> 111 (i.e. 7 in binary so 35 should output 7 in decimal)
*/
void LEDten(int number){
 
    if((number/10)<2){
        number /= 10;
    } else {
        int x = 0;
        x = 1;
        for(int i = 1;i < (number/10); i++){ 
            x = x << 1; //shift bits to left 11 -> 110
            x ++; 		//add 1 110 -> 111 (3 tens)
        }
        number = x;
    }
    
    LEDout(number);
}

void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
}

//void main(void){
//    int buttPush = 0;//make a variable for the number of button pushes
//    int a = 0;//make a variable for the voltage value
//    //a = 0 - ground
//    //a = 1 - 5V (button push
//    LATC = 0;
//    LATD = 0;
//    TRISD = 0;
//    TRISC = 0b00001000; //set pin c3 as input
//    
//    while(1){
//        while(a == 0){
//            a = PORTCbits.RC3;//read from the port
//            
//        }
//        buttPush++; //add to the button Push
//        LEDout(buttPush);//write to the LEDs as well
//        while(a == 1){
//            a = PORTCbits.RC3;
//            
//        }
    
//    }
//}


// main code for ex 2b. and not 2a.! - a above
// increment tens counter (add led) for every button push
void main(void) {
    int a = 0; //read from port c3
    int t = 0; //counter
    int buttPush = 0; //counter for the number of button pushes
    
	//init
    LATC = 0; 
    LATD = 0;
    TRISD = 0;
    TRISC = 0b00001000; //set pin c3 as input
    
    while(1){
        while(a == 0){
            t=0;
            a = PORTCbits.RC3;//read from the port
        }
        buttPush++; //add to the button Push
        LEDten(buttPush);//write to the LEDs as well
        while(a == 1){
            a = PORTCbits.RC3;
            t ++;//increment the time counter
            if(t>300){      //once the time counter reaches 1000
                buttPush++;  //add to the button push counter
                LEDten(buttPush); //display on the LEDs
                delay(70); //delay so the increments are visible.
            }
                 
        }
        if(buttPush > 127){
            buttPush = 0; //dont excede 8 bit limit
        }
    }    
}
