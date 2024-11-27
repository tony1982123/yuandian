#include "USART.h"
//#define Debug


#ifdef Debug
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
	USART1->TDR=ch;
	return (ch);
}
#endif 



/*---------------------------------------------------------------------
FuncName: USART1_Init
Describe: 串口1初始化
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GpioInitStruct;
	USART_InitTypeDef UsartInitStruct;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PB6成第二功能引脚	TX
	

	GpioInitStruct.GPIO_Pin = GPIO_Pin_9 ;
	GpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
	GpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GpioInitStruct.GPIO_OType = GPIO_OType_PP;
	GpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GpioInitStruct);
	

	/* USART1的基本配置 */
	UsartInitStruct.USART_BaudRate = baud;              //波特率
	UsartInitStruct.USART_WordLength = USART_WordLength_8b;
	UsartInitStruct.USART_StopBits = USART_StopBits_1;
	UsartInitStruct.USART_Parity = USART_Parity_No;
	UsartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UsartInitStruct.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &UsartInitStruct);		
		
	/* 使能串口 */
	USART_Cmd(USART1, ENABLE);                            
}

/*---------------------------------------------------------------------
FuncName: USART1_IRQHandler
Describe: 
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void USART1_IRQHandler(void)
{

			
}
#else
void USART1_Init(uint32_t baud)
{

}
#endif



