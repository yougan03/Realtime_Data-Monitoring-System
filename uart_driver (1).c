#include <LPC21xx.H>
#include<stdio.h>
#include "header.h"
void uart0_init(u32 baud)
{
int a[]={15,60,30,15,15};          
unsigned int result=0,pclk;
PINSEL0|=0X5;
pclk=a[VPBDIV]*1000000;
result=pclk/(16*baud);
U0LCR=0X83;
U0DLL=result&0xff;
U0DLM=(result>>8)& 0xff;
U0LCR=0x03;
}

#define THRE ((U0LSR>>5)&1)
void uart0_tx(u8 data)				  
{
U0THR=data;
while(THRE==0);
}


#define RDR (U0LSR&1)
u8 uart0_rx(void)
{										
while(RDR==0);
return U0RBR;
}

 void uart0_tx_string(char *ptr)			   
 {
 while(*ptr!='\0')
 {
 U0THR=*ptr;
while(THRE==0);
ptr++;
 }
 }

void uart0_rx_string(s8 *ptr, u8 len)
{

s32 i;
for(i=0;i<len;i++)
{
while(RDR==0);
ptr[i]=U0RBR;
if(ptr[i]=='\r')
break;
}
ptr[i]='\0';
}

void uart0_tx_float(float num)
{
char a[10];
sprintf(a,"%.1f",num);
uart0_tx_string((s8 *)a);
}

void uart0_tx_integer(s32 num)
{
char a[10];
sprintf(a,"%d",num);
uart0_tx_string((s8 *)a);
}
