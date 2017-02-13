#include "s5pv210.h"
#include "stdio.h"


void delay(void);
void water1(void);
void water2(void);
void water3(void);
void rand_flash(void);
volatile int j = 0; 

void delay(void)
{
	volatile int count=0x100000;
	while(count--);

}

void flash(int i)
{
		rGPJ2DAT &=~(0x1<<i);
		delay();
		rGPJ2DAT |=0x1<<i;
		delay();
}

void water1(void)
{
	
	volatile int i = 0;
	for(i=0;i<4;i++)
	{
	rGPJ2DAT &=~(0x1<<i);
	delay();
	rGPJ2DAT |=0x1<<i;
	delay();
	if(j!=16)
	{
		rGPJ2DAT |= 0xf;
		break;
	}
	}
}

void water2(void)
{
	volatile int i = 0;
	for(i=0;i<3;i++)
	{
		rGPJ2DAT &=~(0x3<<i);
		delay();
		rGPJ2DAT |=(0X3<<i);
	if(j!=64)
	{
		rGPJ2DAT |= 0xf;
		break;
	}
	}
	
}

void water3(void)
{
	volatile int i = 0;
	for(i=0;i<4;i++)
	{
	rGPJ2DAT &=~(0xF);
	rGPJ2DAT |=0x1<<i;
	delay();
	if(j!=32)
	{
		rGPJ2DAT |= 0xf;
		break;
	}
	}
}

void rand_flash(void)
{
	volatile int i = 0;
	int rand_num[20]={3,2,1,3,1,3,2,0,1,1,2,3,2,3,0,2,0,2,0,0};
	for(i=0;i<20;i++)
	{
		rGPJ2DAT &=~(0x1<<rand_num[i]);
		delay();
		rGPJ2DAT |=(0X1<<rand_num[i]);
		if(j!=128)
		{
		rGPJ2DAT |= 0xf;
		break;
		}
	}
}

void func(int i)
{
	switch(i)
	{
		case 1:
			flash(0);
			break;
		case 2:
			flash(1);
			break;
		case 4:
			flash(2);
			break;
		case 8:
			flash(3);
			break;
		case 16:
			water1();
			break;
		case 32:
			water3();
			break;
		case 64:
			water2();
			break;
		case 128:
			rand_flash();
			break;
		default:
		    j=0;
			break;
	}
	
}

void EINT16_31_Handler(void)
{
	volatile int i = 0;
	j = 0;
	for(i=0;i<4;i++)
	{
		if(rEXT_INT_2_PEND & (0x1<<i))
		{
			j |= (0x1<<(i));
			rEXT_INT_2_PEND|=(0x1<<i);
		}
	}
	for(i=0;i<4;i++)
	{
		if(rEXT_INT_3_PEND & (0x1<<i))
		{
		
			j |= (0x1<<(i+4));
			rEXT_INT_3_PEND|=(0x1<<i);
		
		}
	}
	rVIC0ADDRESS=0;
	rVIC1ADDRESS=0;	
	rVIC2ADDRESS=0;
	rVIC3ADDRESS=0;		
}

int main(void)
{
	rGPJ2CON &=~0xFFFF;	//清零[3:0]
	rGPJ2CON |= 0x1111;
	rGPJ2DAT |=0xF;
	
	
	/*	配置GPH2_0为外部中断模式,EINT16 */
	rGPH2CON |=0xFFFF;
	rGPH3CON |=0xFFFF;
	
	
	/*	配置外部中断16-19为下降沿触发 */
	rEXT_INT_2_CON &=~0x7777;
	rEXT_INT_2_CON |= 0x2222;
	rEXT_INT_3_CON &=~0x7777;
	rEXT_INT_3_CON |= 0x2222;
	
	/*	允许部中断16产生中断 */
	rEXT_INT_2_MASK &=~0xf;
	rEXT_INT_3_MASK &=~0xf;
	
	/*	安装中断服务函数 */
	rVIC0VECTADDR16 = (unsigned int)EINT16_31_Handler;
	
	/*	使能VIC0通道16产生中断 */
	rVIC0INTENABLE |=1<<16;
	/*	设置VIC0通道16的中断类型为IRQ中断 */
	rVIC0INTSELECT &=~(1<<16);
	j=0;
    while(1)
    {
		if(j)
			func(j);
	}
        

    return 0;
}



void raise()
{
	
}
