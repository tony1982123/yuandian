#include "hc595.h"
#include "delay.h"

unsigned char LED_0F[] = 
{
//  0	 1	  2	   	  	 	
	0xC0,0xF9,0xA4,
//	3	 4	  5
	0xB0,0x99,0x92,
//	6	 7	  8	
	0x82,0xF8,0x80,
//  9	 A	  b	
	0x90,0x88,0x83,
//  C    d	  E	
	0xC6,0xA1,0x86,
//	F    H	   U
	0x8E,0x89,0xC1,
//	N    
	0xC8,
//	-	 .	  �ر�
	0xbf,0x7F,0xFF
};

// ȫ�ֱ���
unsigned char LED[8];	//����LED��8λ��ʾ����


/*ģ�鴮������*/
static void hc59_shift(uint8_t data)
{
	uint8_t i=0;

	for(i=8;i>=1;i--)
	{
		
		if(data&0x80)  							   
			HC595_SER_SET; 	//si_set;	
		else				
			HC595_SER_CLR; 	//si_clr;	
		
		data=data<<1; 		//data����һλ
		
		HC595_SCK_SET;		//��
		
		HC595_SCK_CLR;		//��	
		
	} 
}
/*ģ�鴮������*/
static void hc59_display(void)
{
	HC595_RCK_SET; 
	HC595_RCK_CLR;
}

void SIG_Display(uint8_t seg,uint8_t data ,uint8_t doc_flag)
{
	unsigned char  *led_table;          // ���ָ��
	unsigned char param;
	switch(data)
	{
		case 0:
			led_table = LED_0F + 0;
			param = *led_table;
		break;
			
		case 1:
			led_table = LED_0F + 1;
			param = *led_table;
		break;		
		
		case 2:
			led_table = LED_0F + 2;
			param = *led_table;
		break;
		case 3:
			led_table = LED_0F + 3;
			param = *led_table;
		break;
		case 4:
			led_table = LED_0F + 4;
			param = *led_table;
		break;
		
		case 5:
			led_table = LED_0F + 5;
			param = *led_table;
		break;
		case 6:
			led_table = LED_0F + 6;
			param = *led_table;
		break;
		case 7:
			led_table = LED_0F + 7;
			param = *led_table;
		break;
		case 8:
			led_table = LED_0F + 8;
			param = *led_table;
		break;
		case 9:
			led_table = LED_0F + 9;
			param = *led_table;
		break;
		case 10://A
			led_table = LED_0F + 10;
			param = *led_table;
		break;
		
		case 11://b
			led_table = LED_0F + 11;
			param = *led_table;
		break;
		
		case 12://c
			led_table = LED_0F + 12;
			param = *led_table;
		break;
		
		case 13://d
			led_table = LED_0F + 13;
			param = *led_table;
		break;
		case 14://E
			led_table = LED_0F + 14;
			param = *led_table;
		break;
		
		case 15://F
			led_table = LED_0F + 15;
			param = *led_table;
		break;
		
		case 16://H
			led_table = LED_0F + 16;
			param = *led_table;
		break;
		
		case 17://U
			led_table = LED_0F + 17;
			param = *led_table;
		break;
		
		case 18://U
			led_table = LED_0F + 18;
			param = *led_table;
		break;
		
		case 19://-
			led_table = LED_0F + 19;
			param = *led_table;
		break;
		
		case 20://.
			led_table = LED_0F + 20;
			param = *led_table;
		break;
		
		case 21://�ر�
			led_table = LED_0F + 21;
			param = *led_table;
		break;
	}
	if((doc_flag == 1)&&(data != 21))//С����ʹ��
		param &= 0x7F;
	//����
	hc59_shift(param);
	//���
	if(seg == 1)
		hc59_shift(seg);
	else if(seg == 2)
		hc59_shift(2);
	else if(seg == 3)
		hc59_shift(4);
	else if(seg == 4)
		hc59_shift(8);
	
	hc59_display();	
}

void Hc595_Init(void)
{	
	GPIO_InitTypeDef GpioInitStruct;
	
	GpioInitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GpioInitStruct.Pull = GPIO_PULLUP; 
	GpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	GpioInitStruct.GPIO_OType = GPIO_OType_PP;
//	GpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	HAL_GPIO_Init(GPIOA, &GpioInitStruct);
	
	HC595_SCK_CLR;
	HC595_RCK_CLR;
	HC595_SER_CLR;
	
	SIG_Display(4,21,0);
	SIG_Display(3,0,0);
	SIG_Display(2,15,0);
	SIG_Display(1,15,0);
	
}


#if 0
void LED_Display (void)
{
	unsigned char  *led_table;          // ���ָ��
	unsigned char i;
	//��ʾ��1λ
	led_table = LED_0F + LED[0];
	i = *led_table;

	hc59_shift(i);			
	hc59_shift(0x01);		
	hc59_display();
	//��ʾ��2λ
	led_table = LED_0F + LED[1];
	i = *led_table;

	hc59_shift(i);		
	hc59_shift(0x02);		
	hc59_display();
	
	//��ʾ��3λ
	led_table = LED_0F + LED[2];
	i = *led_table;

	hc59_shift(i);			
	hc59_shift(0x04);	
	hc59_display();
	
	//��ʾ��4λ
	led_table = LED_0F + LED[3];
	i = *led_table;

	hc59_shift(i);			
	hc59_shift(0x08);		
	hc59_display();
}

void LED4_Display (void)
{
	unsigned char  *led_table;          // ���ָ��
	unsigned char i;
	
	//��ʾ��1λ
	led_table = LED_0F + LED[0];
	i = *led_table;

	hc59_shift(i);			
	hc59_shift(0x01);		
	hc59_display();
	
}

void LED_TEST(void) 
{
	int i = 0;
	LED[0]=1;
	LED[1]=2;
	LED[2]=3;
	LED[3]=4;

	while(1)
	{
//		for(i = 0;i<18;i++)
//		{
//			LED[0]=16;
//			LED4_Display ();
//			delay_ms(1000);
//		}
		//LED_Display();
		SIG_Display(4,18,1);
		
	} 
} 


void display(uint8_t flag)
{
	switch((uint8_t)flag)
	{
		case 0://1.��ʾΪOFF
			SIG_Display(4,20,0);
			SIG_Display(3,0,0);
			SIG_Display(2,15,0);
			SIG_Display(1,15,0);
		break;
		case 1://��ʾΪ ON
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,0,0);
			SIG_Display(1,18,0);
		break;
		case 2://��ʾΪ AU���ж�Ϊҹ���������ж�Ϊ���죬���أ�
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,10,0);
			SIG_Display(1,17,0);
		break;
		case 3://1H��autoģʽ�ж�Ϊҹ��1Сʱ��ػ���auto��
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,1,0);
			SIG_Display(1,16,0);
		break;
		case 4://2H��autoģʽ�ж�Ϊҹ��2Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,2,0);
			SIG_Display(1,16,0);
		break;
		case 5://3H��autoģʽ�ж�Ϊҹ��3Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,3,0);
			SIG_Display(1,16,0);
		break;
		case 6://4H��autoģʽ�ж�Ϊҹ��4Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,4,0);
			SIG_Display(1,16,0);
		break;
		case 7://5H��autoģʽ�ж�Ϊҹ��5Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,5,0);
			SIG_Display(1,16,0);
		break;
		case 8://6H��autoģʽ�ж�Ϊҹ��6Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,6,0);
			SIG_Display(1,16,0);
		break;
		case 9://7H��autoģʽ�ж�Ϊҹ��7Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,7,0);
			SIG_Display(1,16,0);
		break;
		case 10://8H��autoģʽ�ж�Ϊҹ��8Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,8,0);
			SIG_Display(1,16,0);
		break;
		case 11://9H��autoģʽ�ж�Ϊҹ��9Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,20,0);
			SIG_Display(2,9,0);
			SIG_Display(1,16,0);
		break;
		case 12://10H��autoģʽ�ж�Ϊҹ��10Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,1,0);
			SIG_Display(2,0,0);
			SIG_Display(1,16,0);
		break;
		case 13://11H��autoģʽ�ж�Ϊҹ��11Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,1,0);
			SIG_Display(2,1,0);
			SIG_Display(1,16,0);
		break;
		case 14://12H��autoģʽ�ж�Ϊҹ��12Сʱ��ػ���
			SIG_Display(4,20,0);
			SIG_Display(3,1,0);
			SIG_Display(2,2,0);
			SIG_Display(1,16,0);
		break;
	}
}
#endif

