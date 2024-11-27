#include "remote.h"
#include "key_event.h"
#include "func.h"
#include "Userflash.h"

extern SysModeEnum SysMode;
TIM_HandleTypeDef    TimHandle;

/*
https://blog.csdn.net/hallo8888/article/details/84952641

	bit0：400us高电平+800us低电平
	bit1：1ms高电平+200us低电平

	同步码（黑色线条部分）：高电平400us+低电平9ms。
	地址码（橙色线条部分）：20个数据位，共24ms。
	按键码（红色线条部分）：4个数据位，共4.8ms。
*/



//遥控初始化
typedef struct
{
	uint32_t	irtimedata[24]; //遥控接收时间buf	
	uint32_t	irtime;     	//遥控全局变量，计时
	uint8_t		irok;			//接收完成标识
	uint8_t		startflag;  	//遥控启动标识
	uint8_t		ircnt;			//遥控计数器值
	uint32_t	timout_cnt; 	//超时计数器
	uint8_t 	Step; 			//步骤
	uint8_t		SingeLock;		//单次锁
}RemoteMsgType;

RemoteMsgType IrMsg = {0};

extern KeyStateEnum KeyState;


/*---------------------------------------------------------------------
FuncName: 外部中断初始化
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
	GPIO_Struct.Mode = GPIO_MODE_IT_RISING_FALLING;	//输入
	GPIO_Struct.Pull = GPIO_PULLUP;	//拉高
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
	EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//上升下降沿
	//中断线0
	EXTI_Struct.EXTI_Line = EXTI_Line0;	//中断线0
	EXTI_Init(&EXTI_Struct);
	//中断线9
	EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
	EXTI_Struct.EXTI_Line = EXTI_Line9;	//中断线9
	EXTI_Init(&EXTI_Struct);

	//外部中断0优先级0
	NVIC_Struct.NVIC_IRQChannel = EXTI0_1_IRQn;	//顶层头文件重定义好的
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Struct.NVIC_IRQChannelPriority = 0x00;	//中断优先级
	NVIC_Init(&NVIC_Struct);
	//外部中断9优先级2
	NVIC_Struct.NVIC_IRQChannel = EXTI4_15_IRQn;	//顶层头文件重定义好的
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Struct.NVIC_IRQChannelPriority = 0x02;	//中断优先级
	NVIC_Init(&NVIC_Struct);*/
}

void EXTI4_15_IRQHandler(void)	//启动文件中定义好的
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET) 
	{
		//超时处理，等待信号稳定
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
		
		sys_data_write(ADDR_FLASH_PAGE15,(uint16_t*)&SysMode,1);
	}
}

void EXTI0_1_IRQHandler(void)	//启动文件中定义好的
{
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) 
	{
		//超时处理，等待信号稳定
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		
		if(IrMsg.irok == 1)
			return;
			
		IrMsg.startflag = 1;
		if(RF_Dat == 1)//保存低电平时间
		{
			switch(IrMsg.Step)
			{
				case 0:
					//引导码 头码，12.5ms
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
			
				//数据保存 
				case 1:
				{
					IrMsg.irtimedata[IrMsg.ircnt]=IrMsg.irtime;//存储每个电平的持续时间，用于以后判断是0还是1
					IrMsg.irtime=0;
					IrMsg.ircnt++;
					if(IrMsg.ircnt==24)//代表接收完成，24
					{
						IrMsg.irok=1;//可以进行解码处理
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


//timer 时钟最高48M
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM3时钟使能    
	  
	TimeBaseStruct.TIM_Prescaler=48;  //定时器分频
	TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TimeBaseStruct.TIM_Period=99;   //自动重装载值
	TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TimeBaseStruct);//初始化定时器3
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM14,ENABLE); //使能定时器3
	
	
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
	//进行超时 9.4ms + 24ms + 4.8ms
	IrMsg.irtime++;
	if(IrMsg.timout_cnt++ >=500)//125ms
	{
		IrMsg.timout_cnt = 500;
		IrMsg.SingeLock = 0;
	}
}
/*void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update) != RESET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位	
		
		KeyTimeGroup();
		
		SysStateChangeHand();
		
		CountdownTimeHand();
		
		IrDelayDecet();
		
		//进行超时 9.4ms + 24ms + 4.8ms
		IrMsg.irtime++;

		if(IrMsg.timout_cnt++ >=500)//125ms
		{
			IrMsg.timout_cnt = 500;
			IrMsg.SingeLock = 0;
		}
		
	}
}*/




/*---------------------------------------------------------------------
函数名:Remote_Init
描  述:解码初始化，采用外部中断和定时器，定时器125us中断一次，引脚
			下降沿触发
输  入:
输  出:
创始人: DingYi.Zhao
日  期: 
其  他:
---------------------------------------------------------------------*/
void Remote_Init(void)
{
	//外部中断初始化
	exti_init();
	
	//定时器初始化成100us一次中断
	Timer1_Init();//100us中断一次
}


/*------------------------------------------------
                码值处理
------------------------------------------------*/
uint32_t Cordpro(void)//码值处理函数
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


