#include "stm32f10x.h"
#include "Delay.h"



void Delay_1us(void)       
{
	u16 i;
	for(i=9;i>0;i--)        
	{
	}
}


void Delay_nus(u16 n)       
{
	u16 i;
	for(i=n;i>0;i--)
	{
		Delay_1us();
	}
}


void Delay_1ms(void)       
{
	u16 i;
	for(i=0x2328;i>0;i--)    
	{
	}
}

void Delay_nms(u16 n)       
{
	u16 i;
	for(i=n;i>0;i--)
	{
		Delay_1ms();
	}
}
