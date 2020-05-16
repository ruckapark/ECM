#include <xc.h>

#pragma config OSC=IRCIO



void LEDout(int number)
{   
    LATC = ((number & 0b00111100)<<2)|(LATC & 0b00001111); //set all the C pins we want to be a 8bit number
    LATD = ((number & 0b11000000)>>2)|((number & 0b00000011)<<2)|(LATD & 0b11000011); //just combine the part of the d reg split
}

void LEDout10(int number)
{
    LATC = ((number & 0b00111100)<<2)|(LATC & 0b00001111);
    LATD = (((number & 0b1111000000)>>2)|((number & 0b00000011)<<2))|(LATD & 0b00000011);
}

/* Function returns x raised to power n */
double power ( double x, int n ) {
    // initialise y as 1 for if n = 0, returns correctly
    double y = 1;
    
    // multiply by x (n - 1) times
    for (int i = 1; i < n; x++) {
        y *= x;
    }
    
    return y;
}


/*volume detection defined for my voice*/
/*

A potentially neater solution but unecessary for now

    - Give conversion constants ax+b=y a and b.
    - 997 -> 1023 in scale
    - 306 should equal 1
    
    - a = 9/691;
    - b = -2063/691;
	
    - number = ((9*number - 2063)/691)+1; //add one as max volume achievable
    - convert it to binary 2^n-1
    - e.g. 10 = 2^10 - 1
    - number = (2^number)-1;
	*/
int LEDConvertVolume(int number) {
    int volume = 0;
    //int top = 997;
    //int top = 306;

    if (number > 941) {
        number = 0b1111111111;
    } else if (number < 940 && number >= 884) {
        number = 0b0111111111;
    } else if (number < 883 && number >= 828) {
        number = 0b0011111111;
    } else if (number < 827 && number >= 771) {
        number = 0b0001111111;
    } else if (number < 770 && number >=  715) {
        number = 0b0000111111;
    } else if (number < 714 && number >=  658) {
        number = 0b0000011111;
    } else if (number < 657 && number >= 602) {
        number = 0b0000001111;
    } else if (number < 601 && number >= 545) {
        number = 0b0000000111;
    } else if (number < 544 && number >= 489) {
        number = 0b0000000011;
    } else {
        number = 0b0000000001;
    }
    volume = number;
    return volume;
}

void main(void){
    int ADResult = 0; //result of ADC
    int volume = 0;
    int volumeold = 0;
    int hold = 0;
    int t = 0;
    
    LATC = 0;
    LATD = 0;
    TRISA = 0b00001000;
    TRISC = 0;
    TRISD = 0;
    
    ANSEL0 = 0b00001000;
    ANSEL1 = 0;
    
    /* Init ADC (analogue to dig conv) */
    ADCON0 = 0b00001101;
    ADCON1 = 0b00000000;
    ADCON2 = 0b10101011;
    
    while(1){
        ADCON0bits.GO = 1; 		//start conversion

        while (ADCON0bits.GO);  //finish conversion

        ADResult = ADRESL;
        ADResult += ((unsigned int)ADRESH << 8);

        //convert into 10 bit scale of volume
        volume = LEDConvertVolume(ADResult);
        
		
		/*Update old volume for gradual decrease visually*/
        if(hold > volume){ //check if the one bit is bigger than the light
            LEDout10(hold | volume);
            t++;
            if(t>10){
                hold = hold>>1;
            }
        } else{
            LEDout10(volume);
        }
        
        //LEDout10(hold | volume);
        if (volume > volumeold) {
            hold = (volume+1)>>1;
            t=0;
        }
        
        volumeold = volume;
    }
}