#include "main.h"
#include "func.h"
#include "key_event.h"
#include "hc595.h"
#include "string.h"
#include "mixt.h"

SysModeEnum SysMode = Mode0;
SysStateEnum SysState = AdjustState;

TimeMsgType TimeMsg = {0};
extern KeyStateEnum KeyState ;


uint8_t DocFlickerFlag = 0;
uint8_t TimingStep = 0;//执行步骤
IRMsgType IRMsg = {0};

uint8_t ModeChangeFlag = 0;//模式改变标识
uint8_t AutoEnterModeStart = 1;//超时自动进入启动,置1 烧录后要启动
uint32_t AutoEnterModeCount = 0;//超时自动进入
uint8_t IrState = 0xFF;//红外状态
uint8_t EliminateState = 0xFF;//消抖状态
/*---------------------------------------------------------------------
FuncName: SysStateChangeHand
Describe: 系统状态切换
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void SysStateChangeHand(void)
{	
	if(AutoEnterModeStart == 1)
	{
		if(AutoEnterModeCount++ >= 30000)//3S
		{
			AutoEnterModeCount = 0;
			ModeChangeFlag = 1;
			AutoEnterModeStart = 0;
		}
	}
}

/*---------------------------------------------------------------------
FuncName: 
Describe: 
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void EnableMode(void)
{
	if(ModeChangeFlag != 1)
		return ;
	
	ModeChangeFlag = 0;
	
	SysState = DisplayState;//进入显示模式
	TimingStep = 0;//倒计时步骤清零
	IrState = 0xFF;//IR状态
	EliminateState = 0xFF;//消抖状态
	memset(&TimeMsg,0,sizeof(TimeMsg));
	switch((uint8_t)SysMode)
	{
		case Mode0: break;
		case Mode1: break;
		case Mode2: break;
		case Mode3:TimeMsg.Totaltime = TIME_1H;break;//1H（auto模式判断为夜晚，1小时后关机，auto）
		case Mode4:TimeMsg.Totaltime = TIME_2H;break;//2H（auto模式判断为夜晚，2小时后关机）
		case Mode5:TimeMsg.Totaltime = TIME_3H;break;//3H（auto模式判断为夜晚，3小时后关机）
		case Mode6:TimeMsg.Totaltime = TIME_4H;break;//4H（auto模式判断为夜晚，4小时后关机）
		case Mode7:TimeMsg.Totaltime = TIME_5H;break;//5H（auto模式判断为夜晚，5小时后关机）
		case Mode8:TimeMsg.Totaltime = TIME_6H;break;//6H（auto模式判断为夜晚，6小时后关机）
		case Mode9:TimeMsg.Totaltime = TIME_7H;break;//7H（auto模式判断为夜晚，7小时后关机）
		case Mode10:TimeMsg.Totaltime = TIME_8H;break;//8H（auto模式判断为夜晚，8小时后关机）
		case Mode11:TimeMsg.Totaltime = TIME_9H;break;//9H（auto模式判断为夜晚，9小时后关机）
		case Mode12:TimeMsg.Totaltime = TIME_10H;break;//10H（auto模式判断为夜晚，10小时后关机）
		case Mode13:TimeMsg.Totaltime = TIME_11H;break;//11H（auto模式判断为夜晚，11小时后关机）
		case Mode14:TimeMsg.Totaltime = TIME_12H;break;//12H（auto模式判断为夜晚，12小时后关机）
	}	
}

/*---------------------------------------------------------------------
FuncName: ModeAdjust
Describe: 模式调节
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void ModeAdjust(void)
{
	if(KeyState == KeyUp)
	{
		SysMode++;
		if(SysMode > Mode14)
			SysMode = Mode0;
		
		SysState = AdjustState;
		//启动自动进入模式切换
		AutoEnterModeStart = 1;
		AutoEnterModeCount = 0;
	}
	else if(KeyState == KeyDown)
	{
		SysMode--;
		if(SysMode == ModeMax)
			SysMode = Mode14;
		
		SysState = AdjustState;
		//启动自动进入模式切换
		AutoEnterModeStart = 1;
		AutoEnterModeCount = 0;
	}
	
	else if(KeyState == KeyOk)
	{
		//模式切换
		SysState = AdjustState;
		//关闭自动进入模式切换
		AutoEnterModeStart = 0;
		AutoEnterModeCount = 0;
		
		ModeChangeFlag = 1;//按键进入使能模式
	}
	
	KeyState = KeyDefault;//安装状态释放
	//printf("Mode:%d\r\n",SysMode);
	if(SysState != AdjustState)
		return ;
		
		
	switch((uint8_t)SysMode)
	{
		case Mode0://显示为OFF
			SIG_Display(4,21,0);
			SIG_Display(3,0,0);
			SIG_Display(2,15,0);
			SIG_Display(1,15,0);
		break;
		case Mode1://显示为 ON
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,0,0);
			SIG_Display(1,18,0);
		break;
		case Mode2://显示为 AU（判断为夜晚，常开，判断为白天，常关）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,10,0);
			SIG_Display(1,17,0);
		break;
		case Mode3://1H（auto模式判断为夜晚，1小时后关机，auto）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,1,0);
			SIG_Display(1,16,0);
		break;
		case Mode4://2H（auto模式判断为夜晚，2小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,2,0);
			SIG_Display(1,16,0);
		break;
		case Mode5://3H（auto模式判断为夜晚，3小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,3,0);
			SIG_Display(1,16,0);
		break;
		case Mode6://4H（auto模式判断为夜晚，4小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,4,0);
			SIG_Display(1,16,0);
		break;
		case Mode7://5H（auto模式判断为夜晚，5小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,5,0);
			SIG_Display(1,16,0);
		break;
		case Mode8://6H（auto模式判断为夜晚，6小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,6,0);
			SIG_Display(1,16,0);
		break;
		case Mode9://7H（auto模式判断为夜晚，7小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,7,0);
			SIG_Display(1,16,0);
		break;
		case Mode10://8H（auto模式判断为夜晚，8小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,8,0);
			SIG_Display(1,16,0);
		break;
		case Mode11://9H（auto模式判断为夜晚，9小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,9,0);
			SIG_Display(1,16,0);
		break;
		case Mode12://10H（auto模式判断为夜晚，10小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,1,0);
			SIG_Display(2,0,0);
			SIG_Display(1,16,0);
		break;
		case Mode13://11H（auto模式判断为夜晚，11小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,1,0);
			SIG_Display(2,1,0);
			SIG_Display(1,16,0);
		break;
		case Mode14://12H（auto模式判断为夜晚，12小时后关机）
			SIG_Display(4,21,0);
			SIG_Display(3,1,0);
			SIG_Display(2,2,0);
			SIG_Display(1,16,0);
		break;
	}
	
}




static void CountdownTimeDispaly(uint32_t time)
{
	uint8_t hour,min,param4,param3,param2,param1;
	hour = time/60;
	min = time%60;
	
	param4 = hour/10;
	param3 = hour%10;
	
	param2 = min/10;
	param1 = min%10;
	//断码4
	if(param4 != 0)
		SIG_Display(4,param4,0);
	else
		SIG_Display(4,21,0);
	//断码3时间倒数开始时候需要闪烁
	if(TimeMsg.State == 1)
	{
		if(DocFlickerFlag == 0x00)
			SIG_Display(3,param3,1);
		else
			SIG_Display(3,param3,0);
	}
	else//倒数没计时不闪烁
		SIG_Display(3,param3,0);
	//断码2
	SIG_Display(2,param2,0);
	//断码1
	SIG_Display(1,param1,0);
}

/*---------------------------------------------------------------------
FuncName: 
Describe: 
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void CountdownTimeHand(void)
{	
	if((TimeMsg.Totaltime != 0) &&(TimeMsg.State != 0))
	{
		if(TimeMsg.Tick++ >= 599940)//9999*60   1分钟
		{
			TimeMsg.Tick = 0;
			TimeMsg.Totaltime--;
		}
		if(TimeMsg.Tick_1s++ >9999)
		{
			TimeMsg.Tick_1s = 0;
			DocFlickerFlag = ~DocFlickerFlag;
		}
	}
}
/*---------------------------------------------------------------------
FuncName: IrDelayDecet
Describe: 红外延时监测
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void IrDelayDecet(void)
{
	static uint32_t count = 0;
	
	int8_t state = 0;
	
	state = READ_IR;//得到IR状态
	if(EliminateState != state)
	{
		EliminateState = state;
		count = 0;
	}
	else
	{
		if(count++ == 300000)//100us 一次中断，30S = 30*1000000us
		{
			count = 300000;
			IrState = state;
		}
	}
}

/*---------------------------------------------------------------------
FuncName: DisplayFunc
Describe: 显示调节
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void DisplayFunc(void)
{
	if(SysState != DisplayState)
		return ;
	
	if(SysMode == Mode0)
	{
		SIG_Display(4,21,0);
		SIG_Display(3,0,0);
		SIG_Display(2,15,0);
		SIG_Display(1,15,0);
		RELAY_LOW;
	}
	else if(SysMode == Mode1)//显示为 ON
	{
		SIG_Display(4,21,0);
		SIG_Display(3,21,0);
		SIG_Display(2,0,0);
		SIG_Display(1,18,0);
		RELAY_HIGH;
	}
	else if(SysMode == Mode2)
	{//显示为 AU（判断为夜晚，常开，判断为白天，常关）
		SIG_Display(4,21,0);
		SIG_Display(3,21,0);
		SIG_Display(2,10,0);
		SIG_Display(1,17,0);
		
		memset(&TimeMsg,0,sizeof(TimeMsg));
		if(IrState == 0)
			RELAY_HIGH;
		else if(IrState == 1)
			RELAY_LOW;
	}
	else
	{
		switch(TimingStep)
		{
			case 0://0、检测到黑夜，启动时间倒计时
				if(IrState == 0)
				{
					TimeMsg.State = 1;//启动计时
					RELAY_HIGH;
					TimingStep = 1;
				}
				else if(IrState == 1)
				{
					RELAY_LOW;
				}
			break;
			
			case 1://时间到了，关闭继电器
				if(TimeMsg.Totaltime == 0)//当时间到达后，关闭继电器 
				{
					RELAY_LOW;
					TimeMsg.State = 0;//关闭计时标志
					TimeMsg.Tick = 0;//置0 tick
					
					if(IrState == 0)//还是黑夜，此时应该显示000
					{
						//不做处理
					}
					else if(IrState == 1)//白天，则数据进行恢复
					{
						//TimeMsg.Totaltime//恢复总时长
						switch((uint8_t)SysMode)
						{
							case Mode3: TimeMsg.Totaltime = TIME_1H;break;//1H
							case Mode4: TimeMsg.Totaltime = TIME_2H;break;//2H
							case Mode5: TimeMsg.Totaltime = TIME_3H;break;//3H
							case Mode6: TimeMsg.Totaltime = TIME_4H;break;//4H
							case Mode7: TimeMsg.Totaltime = TIME_5H;break;//5H
							case Mode8: TimeMsg.Totaltime = TIME_6H;break;//6H
							case Mode9: TimeMsg.Totaltime = TIME_7H;break;//7H
							case Mode10: TimeMsg.Totaltime = TIME_8H;break;//8H
							case Mode11: TimeMsg.Totaltime = TIME_9H;break;//9H
							case Mode12: TimeMsg.Totaltime = TIME_10H;break;//10H
							case Mode13: TimeMsg.Totaltime = TIME_11H;break;//11H
							case Mode14: TimeMsg.Totaltime = TIME_12H;break;//12H
						}
						TimingStep = 0;
					}
				}
				
			break;
		}
		//倒计时显示
		CountdownTimeDispaly(TimeMsg.Totaltime);
	}
	
}

