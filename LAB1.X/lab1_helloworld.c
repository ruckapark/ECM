#include <xc.h>
#include <stdio.h>
// The microcontroller ?Hello World!? using the UART for output
void main (void)
{
SPEN = 1; // UART module needs to be enabled for printf
TXEN = 1; // to work, will be explained in lab 6
printf("hello world!");
while (1);
}
void putch(char data) // needs to be defined for printf to work
{
 while( ! TXIF); //this will be explained in lab 6
 TXREG = data; //this will be explained in lab 6
}
