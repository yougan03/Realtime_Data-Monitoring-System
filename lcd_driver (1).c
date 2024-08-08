#include <LPC21xx.H>
#include "header.h"
#define RS (1<<17)
#define RW (1<<18)
#define EN (1<<19)

void lcd_data(unsigned char data)
{
	unsigned int temp;
	IOCLR1=0xfe<<16;
	temp=(data&0xf0)<<16;
	IOSET1=temp;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=EN;
	delay_ms(2);
	IOCLR1=EN;
	
	IOCLR1=0xfe<<16;
	temp=(data&0x0f)<<20;
	IOSET1=temp;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=EN;
	delay_ms(2);
	IOCLR1=EN;
}

void lcd_cmd(unsigned char cmd)
{
	unsigned int temp;
	IOCLR1=0xfe<<16;
	temp=(cmd&0xf0)<<16;
	IOSET1=temp;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=EN;
	delay_ms(2);
	IOCLR1=EN;
	
	IOCLR1=0xfe<<16;
	temp=(cmd&0x0f)<<20;
	IOSET1=temp;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=EN;
	delay_ms(2);
	IOCLR1=EN;
}

void lcd_init(void)
{
	IODIR1=0xfe<<16;
	IOCLR1=EN;
	PINSEL2=0x0;
	lcd_cmd(0x02);
	lcd_cmd(0x03);
	lcd_cmd(0x28);
	lcd_cmd(0x0e);
	lcd_cmd(0x01);

}

void lcd_string(char *ptr)
{
	while(*ptr!=0)
 {
	lcd_data(*ptr);
	ptr++;
 }
}


void lcd_cgram(s8 *a)
{
	int i;
	lcd_cmd(0x40);
	for(i=0;i<7;i++)
	{
	 lcd_data(a[i]);
	}
}

void lcd_int(int num)
{
	int a[10];
	int i=0;
	if(num==0)
	{
	 lcd_data('0');
	}
	else if(num<0)
	{
	 lcd_data('-');
	 num=-num;
	}
	while(num>0)
	{
	  a[i]=num%10;
	  num=num/10;
	  i++;
	}
	for(i=i-1;i>=0;i--)
	{
	 lcd_data(a[i]+48);
	}
}

void lcd_float(float f)
 {
	 int i;
	 if(f==0)
	 {
	  lcd_data('0');
	 }
	 if(f<0)
	 {
	  lcd_data('-');
	  f=-f;
	 }
	 i=f;
	 lcd_int(i);
	 lcd_data('.');
	 i=(f-i)*100;
	 lcd_int(i);
 }
