#include <xc.h>

//TRISC = 0b00001000|TRISC;
//TRISCbits.RC3 = 1;   
//set RC3 to be an input 
// which is faster

//to read the port for the button presses, 
//A = PORTCbitsRC3;
//or we could use A = PORTC & 0b00001000;

void LEDout(int number) {

    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split

    //Skeleton function for displaying a binary number
    //on the LED array
    //you can write values to the whole port at once using by
    //changing LATN (e.g. LATC=something;)
    //or
    //individual pins can be changed using
    //LATNbits.LATNx (e.g. LATDbits.LATD2=1;)
}

void LEDten(int number){
 
    if((number/10)<2){
        number /= 10; //below this value it just divides by ten and output in binary
    } else {
        int x = 0;
        x = 1;
        for(int i = 1;i < (number/10); i++){ //for loop runs from i = 1:(n/10-1)
            x = x << 1;//at each loop it shifts to the left 11 << 110
            x ++; //then add a 1 to get 110 = 111
        }
        number = x;
    }
    
    //don't need to worry about floats, will remove decimal.
    LATC = (number & 0b00111100) << 2; //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000) >> 2) | ((number & 0b00000011) << 2); //just combine the part of the d reg split

}

void delay(int t) {
    int x;
    for (x = 0; x < t; x++);
    //Skeleton function for delay code
    //put code in here to make a delay
    //of length t
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

//exercise2


// exercise b


void main(void) {
    int a = 0; //read from port c3
    int t = 0; //counter variable for time button held down
    int buttPush = 0; //counter for the number of button pushes
    
    LATC = 0; //set the output data latch levels to 0 on all pins
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
            buttPush = 0; //make sure it doesnt excede 8 bit limit
        }
    }    
}

//exercise c incorporated into script a