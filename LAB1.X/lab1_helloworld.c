#include <xc.h>
#include <stdio.h>
// print helloworld on microcontroller via UART
void main (void)
{
SPEN = 1; // enable UART for printf to console
TXEN = 1;
printf("hello world!");
while (1);
}
void putch(char data) // without printf not understood
{
 while( ! TXIF); 	//see lab 6 for explanation
 TXREG = data;
}
