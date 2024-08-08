#include <LPC21xx.H>
#include "header.h"
int main()
{
u16 temp;
float vout,temperature;
uart0_init(9600);
adc_init();
lcd_init();
uat0_tx_string("lpc2129 adc testing code\r\n");
while(1)
{
temp=adc_read(1);
vout=((temp*3.3)/1023);
temperature=(vout-0.5)/0.01;
uart0_tx_float(temperature);
lcd_cmd(0xcb);
lcd_float(temperature);
delay_ms(500);
lcd_cmd(0x01);
uart0_tx_string("\r\n");
}

}