#include <LPC21xx.H>
#include "header.h"
#define sw1 ((IOPIN0>>14)&1)
#define sw2 ((IOPIN0>>15)&1)
#define sw3 ((IOPIN0>>16)&1)
u32 flag=0;
u8 h,m,s,p,arg,x;
int temp1,temp2=0,B,temp3,count;
float vout,A,temperature;
s8 a[]={0x08,0x14,0x08,0x07,0x08,0x08,0x07,0x00};



__irq void timer1_handler(void)
{
u32 T;
T = T1IR;
count=1;
T1IR= T;
VICVectAddr=0;
}

__irq void Eint0_handler(void)
{
	flag=1;
	EXTINT=1;
	VICVectAddr=0;
}

	void config_vic(void)
	{
		VICIntSelect=0;
		VICVectAddr0=(u32)timer1_handler;
		VICVectCntl0=5|(1<<5);
		
		//VICIntSelect=0;
		VICVectAddr1=(u32)Eint0_handler;
		VICVectCntl1=14|(1<<5);
		VICIntEnable|=(1<<5)|(1<<14);
	}

void en_eint0(void)
	{
		PINSEL1|=1;
		EXTMODE=1;
		EXTPOLAR=0;
	}


void en_timer1_interrupt(void)
{
s32 a[]={15,60,30,15,15};
u32 pclk=a[VPBDIV]*1000000;
T1MR0=1;
T1MCR=3;
T1PC=0;
T1PR=pclk-1;
T1TC=0;
T1TCR=1;
}

int main()
{
	en_eint0();
	en_timer1_interrupt();
	config_vic();
	adc_init();
	spi0_init();
	lcd_init();
	i2c_init();
	
	i2c_byte_write_frame(0xD0,0x0,0x30);
	i2c_byte_write_frame(0xD0,0x1,0x47);
	i2c_byte_write_frame(0xD0,0x2,0x71);

	 ////********* read temperature from sensor**************//////////////////

while(1)
{
	if(flag==0)
	{
	while(1)
	{
	h=i2c_byte_read_frame(0xD0,0x2);
	m=i2c_byte_read_frame(0xD0,0x1);
	s=i2c_byte_read_frame(0xD0,0x0);
	
	p=h>>5&1;
	h=h&0x1f;
	
	lcd_cmd(0x80);
	lcd_data((h/0x10)+48);
	lcd_data((h%0x10)+48);
	lcd_data(':');
	lcd_data((m/0x10)+48);
	lcd_data((m%0x10)+48);
	lcd_data(':');
	
	lcd_data((s/0x10)+48);
	lcd_data((s%0x10)+48);
	//lcd_data(' ');
	if(p==1)
	lcd_string("pm");
	else
	lcd_string("am");
							//////////////temperature////////////
	
	temp3=adc_read(1);
	temp2=read_mcp3204(1); //ldr
	temp1=read_mcp3204(0);		//pot
	
	A=(temp1*5.0)/4095;
	B=(4095-temp2)*100/4095;
	vout=((temp3*3.3)/1023);
	temperature=(vout-0.5)/0.01;
	if(count==1)
	{
	lcd_float(temperature);				
	count=0;
	}
	
	
	lcd_cgram(a);
	lcd_cmd(0x8f);
	lcd_data(0);			    /////********** pm *********************/////////////////////////
	
	lcd_cmd(0xc0);
	lcd_string("PM:");
	lcd_float(A);
	lcd_string("v ");
	
	lcd_cmd(0xc8);
	lcd_string("LDR:");
	lcd_data((B/100)+48);
	lcd_data(((B/10)%10)+48);
	lcd_data((B%10)+48);

	lcd_cmd(0xcf);
	lcd_data('%');
	
	if(flag==1)
	{
	lcd_cmd(0x01);
	break;
	}
	}
	}
	else if(flag==1)
	{
	lcd_cmd(0x80);
	lcd_string("setting mode");
	delay_ms(100);
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_string("enter hour");
	lcd_cmd(0xc0);
	h=1;
	lcd_data((h/10)+48);
	lcd_data((h%10)+48);
	while(1)
	{
	if(sw1==0)
	{
	while(sw1==0);
	h++;
	if(h==13)
	h=1;
	lcd_cmd(0xc0);
	lcd_data((h/10)+48);
	lcd_data((h%10)+48);
	}
	if(sw2==0)
	{
	while(sw2==0);
	arg=0;
	arg=h%10;
	arg|=((h/10)<<4);
	arg|=(1<<6);
	i2c_byte_write_frame(0xd0,0x2,arg);
	break;
	}
	}
	
	lcd_cmd(0x01);
	lcd_string("enter minute");
	lcd_cmd(0xc0);
	m=0;
	lcd_data((m/10)+48);
	lcd_data((m%10)+48);
	while(1)
	{
	if(sw1==0)
	{
	while(sw1==0);
	m++;
	if(m==60)
	m=1;
	lcd_cmd(0xc0);
	lcd_data((m/10)+48);
	lcd_data((m%10)+48);
	}
	if(sw2==0)
	{
	while(sw2==0);
	arg=0;
	arg=m%10;
	arg|=((m/10)<<4);
	i2c_byte_write_frame(0xd0,0x1,arg);
	break;
	} 
	 }
	
	lcd_cmd(0x01);
	lcd_string("enter second");
	lcd_cmd(0xc0);
	s=0;
	lcd_data((s/10)+48);
	lcd_data((s%10)+48);
	while(1)
	{
	if(sw1==0)
	{
	while(sw1==0);
	s++;
	if(s==60)
	s=1;
	lcd_cmd(0xc0);
	lcd_data((s/10)+48);
	lcd_data((s%10)+48);
	}
	if(sw2==0)
	{
	while(sw2==0);
	arg=0;
	arg=s%10;
	arg|=((s/10)<<4);
	i2c_byte_write_frame(0xd0,0x0,arg);
	break;
	}
	}
	lcd_cmd(0x01);
	lcd_string("enter AM or PM");
	lcd_cmd(0xc0);
	x=0;
	lcd_string("AM");
	while(1)
	{
	if(sw1==0)
	{
	while(sw1==0);
	x^=1;
	if(x==0)
	{
	lcd_cmd(0xc0);
	lcd_string("AM");
	}
	else
	{
	lcd_cmd(0xc0);
	lcd_string("PM");
	}
	}
	if(sw2==0)
	{
	while(sw2==0);
	h=i2c_byte_read_frame(0xd0,0x2);
	h|=(x<<5);
	i2c_byte_write_frame(0xd0,0x2,h);
	break;
	}
	}
	flag=0;
	}
	}
}

