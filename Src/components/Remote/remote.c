#include "remote.h"
#include "key_event.h"
#include "func.h"
#include "Userflash.h"

extern SysModeEnum SysMode;
TIM_HandleTypeDef    TimHandle;

/*
https://blog.csdn.net/hallo8888/article/details/84952641

	bit0��400us�ߵ�ƽ+800us�͵�ƽ
	bit1��1ms�ߵ�ƽ+200us�͵�ƽ

	ͬ���루��ɫ�������֣����ߵ�ƽ400us+�͵�ƽ9ms��
	��ַ�루��ɫ�������֣���20������λ����24ms��
	�����루��ɫ�������֣���4������λ����4.8ms��
*/



//ң�س�ʼ��
typedef struct
{
	uint32_t	irtimedata[24]; //ң�ؽ���ʱ��buf	
	uint32_t	irtime;     	//ң��ȫ�ֱ�������ʱ
	uint8_t		irok;			//������ɱ�ʶ
	uint8_t		startflag;  	//ң��������ʶ
	uint8_t		ircnt;			//ң�ؼ�����ֵ
	uint32_t	timout_cnt; 	//��ʱ������
	uint8_t 	Step; 			//����
	uint8_t		SingeLock;		//������
}RemoteMsgType;

RemoteMsgType IrMsg = {0};

extern KeyStateEnum KeyState;


/*---------------------------------------------------------------------
FuncName: �ⲿ�жϳ�ʼ��
Describe: 
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
static void exti_init(void)
{
	GPIO_InitTypeDef GPIO_Struct;
//	NVIC_InitTypeDef NVIC_Struct;
//	EXTI_InitTypeDef EXTI_Struct;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_Struct.Pin = GPIO_PIN_0 | GPIO_PIN_9;
	GPIO_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Struct.Mode = GPIO_MODE_IT_RISING_FALLING;	//����
	GPIO_Struct.Pull = GPIO_PULLUP;	//����
	HAL_GPIO_Init(GPIOA, &GPIO_Struct);
	
	/* Enable EXTI interrupt */
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
  /* Configure interrupt priority */
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);

	/* Enable EXTI interrupt */
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  /* Configure interrupt priority */
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 2);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);

	
/*	EXTI_Struct.EXTI_LineCmd = ENABLE;
	EXTI_Struct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//�����½���
	//�ж���0
	EXTI_Struct.EXTI_Line = EXTI_Line0;	//�ж���0
	EXTI_Init(&EXTI_Struct);
	//�ж���9
	EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_Struct.EXTI_Line = EXTI_Line9;	//�ж���9
	EXTI_Init(&EXTI_Struct);

	//�ⲿ�ж�0���ȼ�0
	NVIC_Struct.NVIC_IRQChannel = EXTI0_1_IRQn;	//����ͷ�ļ��ض���õ�
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Struct.NVIC_IRQChannelPriority = 0x00;	//�ж����ȼ�
	NVIC_Init(&NVIC_Struct);
	//�ⲿ�ж�9���ȼ�2
	NVIC_Struct.NVIC_IRQChannel = EXTI4_15_IRQn;	//����ͷ�ļ��ض���õ�
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Struct.NVIC_IRQChannelPriority = 0x02;	//�ж����ȼ�
	NVIC_Init(&NVIC_Struct);*/
}

void EXTI4_15_IRQHandler(void)	//�����ļ��ж���õ�
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET) 
	{
		//��ʱ�����ȴ��ź��ȶ�
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
		
		sys_data_write(ADDR_FLASH_PAGE15,(uint16_t*)&SysMode,1);
	}
}

void EXTI0_1_IRQHandler(void)	//�����ļ��ж���õ�
{
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) 
	{
		//��ʱ�����ȴ��ź��ȶ�
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		
		if(IrMsg.irok == 1)
			return;
			
		IrMsg.startflag = 1;
		if(RF_Dat == 1)//����͵�ƽʱ��
		{
			switch(IrMsg.Step)
			{
				case 0:
					//������ ͷ�룬12.5ms
					if(IrMsg.irtime<130&&IrMsg.irtime>=110)//10ms~11ms
					{
						IrMsg.Step = 1;
					}
					else
					{
						IrMsg.Step = 0;
						IrMsg.startflag = 0;
					}
					IrMsg.irok = 0;
					IrMsg.irtime = 0;
					IrMsg.ircnt=0;
				break;
			
				//���ݱ��� 
				case 1:
				{
					IrMsg.irtimedata[IrMsg.ircnt]=IrMsg.irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
					IrMsg.irtime=0;
					IrMsg.ircnt++;
					if(IrMsg.ircnt==24)//���������ɣ�24
					{
						IrMsg.irok=1;//���Խ��н��봦��
						IrMsg.ircnt = 0;
						IrMsg.Step = 0;
					}
					
					IrMsg.timout_cnt = 0;
				}
				break;
			}
		}
		else
		{
			IrMsg.irtime = 0;
		}
		
	}	
}


//timer ʱ�����48M
/*---------------------------------------------------------------------
FuncName: Timer6_Init
Describe: 
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
static void Timer1_Init(void)
{
/*	TIM_TimeBaseInitTypeDef  TimeBaseStruct;
	NVIC_InitTypeDef NVIC_Struct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM3ʱ��ʹ��    
	  
	TimeBaseStruct.TIM_Prescaler=48;  //��ʱ����Ƶ
	TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TimeBaseStruct.TIM_Period=99;   //�Զ���װ��ֵ
	TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TimeBaseStruct);//��ʼ����ʱ��3
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM14,ENABLE); //ʹ�ܶ�ʱ��3
	
	
	NVIC_Struct.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_Struct.NVIC_IRQChannelPriority = 0x01;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);*/
	TimHandle.Instance = TIM1; 
	TimHandle.Init.Period  					 = 300 - 1;
	TimHandle.Init.Prescaler         = 8000 - 1;
	TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP; 
	TimHandle.Init.RepetitionCounter = 1 - 1; 
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)                                
  {
    APP_ErrorHandler();
  }
	HAL_TIM_Base_Start_IT(&TimHandle);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  KeyTimeGroup();
	SysStateChangeHand();
	CountdownTimeHand();
	IrDelayDecet();
	//���г�ʱ 9.4ms + 24ms + 4.8ms
	IrMsg.irtime++;
	if(IrMsg.timout_cnt++ >=500)//125ms
	{
		IrMsg.timout_cnt = 500;
		IrMsg.SingeLock = 0;
	}
}
/*void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update) != RESET) //����ж�
	{
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //����жϱ�־λ	
		
		KeyTimeGroup();
		
		SysStateChangeHand();
		
		CountdownTimeHand();
		
		IrDelayDecet();
		
		//���г�ʱ 9.4ms + 24ms + 4.8ms
		IrMsg.irtime++;

		if(IrMsg.timout_cnt++ >=500)//125ms
		{
			IrMsg.timout_cnt = 500;
			IrMsg.SingeLock = 0;
		}
		
	}
}*/




/*---------------------------------------------------------------------
������:Remote_Init
��  ��:�����ʼ���������ⲿ�жϺͶ�ʱ������ʱ��125us�ж�һ�Σ�����
			�½��ش���
��  ��:
��  ��:
��ʼ��: DingYi.Zhao
��  ��: 
��  ��:
---------------------------------------------------------------------*/
void Remote_Init(void)
{
	//�ⲿ�жϳ�ʼ��
	exti_init();
	
	//��ʱ����ʼ����100usһ���ж�
	Timer1_Init();//100us�ж�һ��
}


/*------------------------------------------------
                ��ֵ����
------------------------------------------------*/
uint32_t Cordpro(void)//��ֵ������
{ 
	unsigned char i;
	uint32_t value = 0xFFFFFF;
	if(!IrMsg.irok)
	{
		return value;
	}
	for(i = 0;i<24;i++)
	{
		if(IrMsg.irtimedata[i] > 7)
			value  |= (1<< i);
		else
			value  &= ~(1<< i);
	}
	
	IrMsg.irok = 0;
	
	return value;
}



void get_ir_state(void)
{
	uint32_t val=0xFFFFFF;
	
	val = Cordpro();
	
	if((val == 0xFFFFFF) || (val == 0x000)) 
	{
		return ;
	}

	//printf("1:val:%x %d\r\n",val,IrMsg.flag);

	switch ((uint32_t)val)
	{
		case RemoteUp:
			if(IrMsg.SingeLock == 0)
			{
				IrMsg.SingeLock = 1;
				KeyState = KeyUp;
			}
		break;
		case RemoteOk:
			if(IrMsg.SingeLock == 0)
			{
				IrMsg.SingeLock = 1;
				
				KeyState = KeyOk;
			}
		break;
		case RemoteDown:
			if(IrMsg.SingeLock == 0)
			{
				IrMsg.SingeLock = 1;
				
				KeyState = KeyDown;
				
			}
		break;
	}
}


