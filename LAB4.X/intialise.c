#include <xc.h>
#include <string.h>
#include <stdio.h>
#pragma config OSC = IRCIO, WDTEN = OFF //internal osciallator, WTD off

#define _XTAL_FREQ 8000000 //allows precise delay functions to work

#define ENABLE_BIT LATCbits.LATC0     //enable bit pin
#define DB7 LATDbits.LATD1            //define the send pins
#define DB6 LATDbits.LATD0
#define DB5 LATCbits.LATC2
#define DB4 LATCbits.LATC1
#define RS LATAbits.LATA6

#define DB7TRIS TRISDbits.RD1            //define the send pins
#define DB6TRIS TRISDbits.RD0
#define DB5TRIS TRISCbits.RC2
#define DB4TRIS TRISCbits.RC1

//i.e. for an 8MHz clock frequency
// precise delays can be produced using the __delay_us() and __delay_ms()



//function to toggle enable bit on then off
//enable bit is connected to pin RC0 or bit E on the LCD display

void E_TOG(void) {
    ENABLE_BIT = 1;
    __delay_us(5); // 5us delay ? remember to define _XTAL_FREQ
    ENABLE_BIT = 0;
}

//function to send four bits to the LCD

void LCDout(unsigned char number) {

    DB7 = number >> 3;
    DB6 = (number & 0b0100) >> 2;
    DB5 = (number & 0b0010) >> 1;
    DB4 = (number & 0b0001);
    //set data pins using the four bits from number
    //toggle the enable bit to send data
    E_TOG();

    __delay_us(5); // 5us delay
}

//function to send data/commands over a 4bit interface

void SendLCD(unsigned char Byte, char type) {
    RS=type;
    // set RS pin whether it is a Command (0) or Data/Char (1)

    // send high bits of Byte using LCDout function
    LCDout(Byte >> 4);
    __delay_us(10); // 10us delay
    // send low bits of Byte using LCDout function
    LCDout(Byte & 0x0f);
    __delay_us(10); // 10us delay
}

void LCD_Init(void) {
    // set initial LAT output values (they start up in a random state)
    RS = 0;
    ENABLE_BIT = 0;
    DB4 = 0;
    DB5 = 0;
    DB6 = 0;
    DB7 = 0;
    // set LCD pins as output (TRIS registers)
    
    TRISAbits.RA6 = 0;
    TRISCbits.RC0 = 0;
    DB4TRIS = 0;
    DB5TRIS = 0;
    DB6TRIS = 0;
    DB7TRIS = 0;
    // Initialisation sequence code - see the data sheet
    //delay 15mS
    __delay_ms(15);
    //send 0b0011 using LCDout
    LCDout(0b0011);
    //delay 5ms
    __delay_ms(5);
    //send 0b0011 using LCDout
    LCDout(0b0011);
    //delay 200us
    __delay_us(200);
    //send 0b0011 using LCDout
    LCDout(0b0011);
    //delay 50us
    __delay_us(50);
    //send 0b0010 using LCDout set to four bit mode
    LCDout(0b0010);
    __delay_us(50);
    // now use SendLCD to send whole bytes ? send function set, clear
    SendLCD(0b00101000, 0);
    __delay_us(50);
    SendLCD(0b00001000, 0);
    __delay_us(50);
    SendLCD(0b00000001, 0); //curse moves up and display not shifted
    __delay_ms(5);
    SendLCD(0b00000110, 0);
    __delay_us(50);
    SendLCD(0b00001110, 0);// screen, set entry mode, display on etc to finish initialisation
    __delay_us(50);
}


//function to put cursor to start of line

void SetLine(char line) {
    //Send 0x80 to set line to 1 (0x00 ddram address)
    if(line==1){
    SendLCD(0x80,0);}
    //Send 0xC0 to set line to 2 (0x40 ddram address)
    else{SendLCD(0xC0,0);}
    __delay_us(50); // 50us delay
}

// function to send entire string with pointers
void LCD_String(char *string) {
    while(*string != 0) {                   //string ends in zero
        SendLCD(*string++,1);               //move through string characters
        __delay_us(50);     
    }
}

//function that sends the result to the LCD display in the interval of 2-5 volts.
void intAndFracDisplay(int ADResult){
    char buf[16];                                   //allocate memory
                                                    
    int intPart = (ADResult/203.6);                 //(1023 limit)/5 = 203.6
    
    int fracPart = (ADResult/2.036) - intPart*100;  //find the fractional part to 2dp
    
    sprintf(buf,"%d.%02dV",intPart,fracPart);      //display it using the sprint function
    SendLCD(0b00000001,0);
    __delay_ms(3);
    
    LCD_String(buf);                                //send to the LCD display
}

void main(void) {
    char buf[16];
    
    OSCCON = 0x72; //8MHz Clock
    while (!OSCCONbits.IOFS); //Wait for OSC to become stable

    LCD_Init();

    SetLine(1); //Set Line 1
//    SendLCD(0x48, 1); //H
    
   
    strcpy(buf,"Hello World! How are you doing today?");
    LCD_String(buf);
    
    //SetLine(2);
    //LCD_String("again!");
    //int ADResult = 0;
    
    while (1) {
        for (int i = 0; i < 5; i++){__delay_ms(50);}
        SendLCD(0b00011000, 0);
        __delay_us(50);
    }
    
//    TRISAbits.RA3 = 1;
//    
//    ANSEL0 = 0b00001000;
//    ANSEL1 = 0;
//    
//    /* Init ADC */
//    ADCON0 = 0b00001101;
//    ADCON1 = 0b00000000;
//    ADCON2 = 0b10101011;
//    
//    while(1){
//        for (int i = 0; i < 5; i++){__delay_ms(50);}
//        
//        ADCON0bits.GO = 1; //start conversion
//
//        while (ADCON0bits.GO);  //finish conversion
//
//        ADResult = ADRESL;
//        ADResult += ((unsigned int)ADRESH << 8);
//        
//        intAndFracDisplay(ADResult);
//    }

}