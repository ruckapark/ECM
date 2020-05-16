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

void sendCharSerial(char charToSend){
    while (!PIR1bits.TXIF);     //what for the receive interrupt to clear
    TXREG = charToSend;               //return byte in the RCREG
}

char intAndFracSerial(int ADResult){
    char buf[16]; //allocate memory

    int intPart = (ADResult / 203.6); //(1023 limit)/5 = 203.6

    int fracPart = (ADResult / 2.036) - intPart * 100; //find the fractional part to 2dp

    sprintf(buf, "%d.%02dV", intPart, fracPart); //display it using the sprint function

    char i = 0; //Declare i for this loop
    
    while (buf[i] != 0) { //Look for null character to finish the loop
        sendCharSerial(buf[i]); //Work along buf sending its characters
        i++; //Probably better way of doing this
    }
    
    sendCharSerial('\r');
    __delay_ms(50);
}

void main(void){
    OSCCON = 0x72;                  //8MHz Clock
    while (!OSCCONbits.IOFS);       //Wait for OSC to become stable
    
    /*reset the latches and configure the tristates*/
    LATC = 0;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 1;
    
    
    BAUDCONbits.BRG16 = 1; //set baud rate scaling to 16 bit mode
    /*set baud rate to be - 19200 - BRG should be 103*/
    //apparently BRGH is high but i dont think it is
    SPBRG = 102; //low byte
    SPBRGH = 0;  //high byte
    
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continous receive mode
    RCSTAbits.SPEN = 1; //enable serial port, other settings default
    TXSTAbits.TXEN = 1; //enable transmitter, other settings default
    
    ANSEL0 = 0b00001000;
    ANSEL1 = 0;
    
    /* Init ADC */
    ADCON0 = 0b00001101;
    ADCON1 = 0b00000000;
    ADCON2 = 0b10101011;
    
    int ADResult;
    
    while(1){
        //Copy and pasted AD conversion code from Lab 2 Exercise 4a
        ADCON0bits.GO = 1;                       //start conversion

        while (ADCON0bits.GO);                   //finish conversion

        ADResult = ADRESL;
        ADResult += ((unsigned int)ADRESH << 8); //ADResult now a 10 bit binary number

        intAndFracSerial(ADResult);              //Convert the ADResult binary into a string buf
        //Maybe worth putting a delay in  
    }
}
