 #include <LPC21xx.H>
#include"header.h"
#define SPIF ((S0SPSR>>7)&1)
u8 spi0(u8 data)
{
S0SPDR=data;
while(SPIF==0);
return S0SPDR;
}
void spi0_init(void)
{
 PINSEL0|=0X1500;
 IODIR0|=CS0;
 IOSET0=CS0;
 S0SPCR=0X20;
 S0SPCCR=15;
}
