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
    
    while(1){
        sendCharSerial(0x02);
        sendCharSerial(0x65);
        sendCharSerial(0x63);
        sendCharSerial(0x68);
        sendCharSerial(0x6F);
        sendCharSerial(0x03);    
    }
    
    
    
}

