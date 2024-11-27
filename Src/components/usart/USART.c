#include "USART.h"
//#define Debug


#ifdef Debug
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
	USART1->TDR=ch;
	return (ch);
}
#endif 



/*---------------------------------------------------------------------
FuncName: USART1_Init
Describe: ����1��ʼ��
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GpioInitStruct;
	USART_InitTypeDef UsartInitStruct;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//����PB6�ɵڶ���������	TX
	

	GpioInitStruct.GPIO_Pin = GPIO_Pin_9 ;
	GpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
	GpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GpioInitStruct.GPIO_OType = GPIO_OType_PP;
	GpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GpioInitStruct);
	

	/* USART1�Ļ������� */
	UsartInitStruct.USART_BaudRate = baud;              //������
	UsartInitStruct.USART_WordLength = USART_WordLength_8b;
	UsartInitStruct.USART_StopBits = USART_StopBits_1;
	UsartInitStruct.USART_Parity = USART_Parity_No;
	UsartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UsartInitStruct.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &UsartInitStruct);		
		
	/* ʹ�ܴ��� */
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



