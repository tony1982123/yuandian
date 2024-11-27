#include "delay.h"

#if 1
static int8_t  fac_us=0;//us
static int16_t fac_ms=0;//ms

void delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8  
	fac_ms=(int16_t)fac_us*1000;//每个ms需要的systick时钟数   
}	
//延时Nus
void delay_us(int32_t nus)
{		
	int32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时Nms

void delay_ms(int16_t nms)
{	 		  	  
	int32_t temp;		   
	SysTick->LOAD=(int32_t)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
#else

void delay_init()
{
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	//fac_us=SYSCLK/8;		
	SysTick_Config( SystemCoreClock / 1000 );	
}

void delay_us(int32_t us)
{
	uint32_t start,now,delta,reload,us_tick;
	start = SysTick->VAL;
	reload = SysTick->LOAD;
	us_tick = SystemCoreClock/1000000UL;
	do{
		now = SysTick->VAL;
		
		delta = start > now ? start - now:reload + start -now;
	
	}while(delta < us_tick*us);
}

void delay_ms(int16_t nms)
{
	int16_t repeat=nms;						
											
	while(repeat)
	{
		delay_us(500);
		delay_us(500);
		repeat--;
	}
}
#endif

