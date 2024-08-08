#include <LPC21xx.H>
#include "header.h"	 

u16 read_mcp3204(u8 ch_num)
{
	u8 byteH=0,byteL=0;
	u16 result=0;
	ch_num<<=6;
	IOCLR0=CS0;
	spi0(0x06);
	byteH=spi0(ch_num);
	byteL=spi0(0x0);
	IOSET0=CS0;
	byteH&=0X0F;
	result=(byteH<<8)|byteL;
	return result;
}
