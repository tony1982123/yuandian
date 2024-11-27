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
uint8_t TimingStep = 0;//ִ�в���
IRMsgType IRMsg = {0};

uint8_t ModeChangeFlag = 0;//ģʽ�ı��ʶ
uint8_t AutoEnterModeStart = 1;//��ʱ�Զ���������,��1 ��¼��Ҫ����
uint32_t AutoEnterModeCount = 0;//��ʱ�Զ�����
uint8_t IrState = 0xFF;//����״̬
uint8_t EliminateState = 0xFF;//����״̬
/*---------------------------------------------------------------------
FuncName: SysStateChangeHand
Describe: ϵͳ״̬�л�
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
	
	SysState = DisplayState;//������ʾģʽ
	TimingStep = 0;//����ʱ��������
	IrState = 0xFF;//IR״̬
	EliminateState = 0xFF;//����״̬
	memset(&TimeMsg,0,sizeof(TimeMsg));
	switch((uint8_t)SysMode)
	{
		case Mode0: break;
		case Mode1: break;
		case Mode2: break;
		case Mode3:TimeMsg.Totaltime = TIME_1H;break;//1H��autoģʽ�ж�Ϊҹ��1Сʱ��ػ���auto��
		case Mode4:TimeMsg.Totaltime = TIME_2H;break;//2H��autoģʽ�ж�Ϊҹ��2Сʱ��ػ���
		case Mode5:TimeMsg.Totaltime = TIME_3H;break;//3H��autoģʽ�ж�Ϊҹ��3Сʱ��ػ���
		case Mode6:TimeMsg.Totaltime = TIME_4H;break;//4H��autoģʽ�ж�Ϊҹ��4Сʱ��ػ���
		case Mode7:TimeMsg.Totaltime = TIME_5H;break;//5H��autoģʽ�ж�Ϊҹ��5Сʱ��ػ���
		case Mode8:TimeMsg.Totaltime = TIME_6H;break;//6H��autoģʽ�ж�Ϊҹ��6Сʱ��ػ���
		case Mode9:TimeMsg.Totaltime = TIME_7H;break;//7H��autoģʽ�ж�Ϊҹ��7Сʱ��ػ���
		case Mode10:TimeMsg.Totaltime = TIME_8H;break;//8H��autoģʽ�ж�Ϊҹ��8Сʱ��ػ���
		case Mode11:TimeMsg.Totaltime = TIME_9H;break;//9H��autoģʽ�ж�Ϊҹ��9Сʱ��ػ���
		case Mode12:TimeMsg.Totaltime = TIME_10H;break;//10H��autoģʽ�ж�Ϊҹ��10Сʱ��ػ���
		case Mode13:TimeMsg.Totaltime = TIME_11H;break;//11H��autoģʽ�ж�Ϊҹ��11Сʱ��ػ���
		case Mode14:TimeMsg.Totaltime = TIME_12H;break;//12H��autoģʽ�ж�Ϊҹ��12Сʱ��ػ���
	}	
}

/*---------------------------------------------------------------------
FuncName: ModeAdjust
Describe: ģʽ����
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
		//�����Զ�����ģʽ�л�
		AutoEnterModeStart = 1;
		AutoEnterModeCount = 0;
	}
	else if(KeyState == KeyDown)
	{
		SysMode--;
		if(SysMode == ModeMax)
			SysMode = Mode14;
		
		SysState = AdjustState;
		//�����Զ�����ģʽ�л�
		AutoEnterModeStart = 1;
		AutoEnterModeCount = 0;
	}
	
	else if(KeyState == KeyOk)
	{
		//ģʽ�л�
		SysState = AdjustState;
		//�ر��Զ�����ģʽ�л�
		AutoEnterModeStart = 0;
		AutoEnterModeCount = 0;
		
		ModeChangeFlag = 1;//��������ʹ��ģʽ
	}
	
	KeyState = KeyDefault;//��װ״̬�ͷ�
	//printf("Mode:%d\r\n",SysMode);
	if(SysState != AdjustState)
		return ;
		
		
	switch((uint8_t)SysMode)
	{
		case Mode0://��ʾΪOFF
			SIG_Display(4,21,0);
			SIG_Display(3,0,0);
			SIG_Display(2,15,0);
			SIG_Display(1,15,0);
		break;
		case Mode1://��ʾΪ ON
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,0,0);
			SIG_Display(1,18,0);
		break;
		case Mode2://��ʾΪ AU���ж�Ϊҹ���������ж�Ϊ���죬���أ�
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,10,0);
			SIG_Display(1,17,0);
		break;
		case Mode3://1H��autoģʽ�ж�Ϊҹ��1Сʱ��ػ���auto��
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,1,0);
			SIG_Display(1,16,0);
		break;
		case Mode4://2H��autoģʽ�ж�Ϊҹ��2Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,2,0);
			SIG_Display(1,16,0);
		break;
		case Mode5://3H��autoģʽ�ж�Ϊҹ��3Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,3,0);
			SIG_Display(1,16,0);
		break;
		case Mode6://4H��autoģʽ�ж�Ϊҹ��4Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,4,0);
			SIG_Display(1,16,0);
		break;
		case Mode7://5H��autoģʽ�ж�Ϊҹ��5Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,5,0);
			SIG_Display(1,16,0);
		break;
		case Mode8://6H��autoģʽ�ж�Ϊҹ��6Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,6,0);
			SIG_Display(1,16,0);
		break;
		case Mode9://7H��autoģʽ�ж�Ϊҹ��7Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,7,0);
			SIG_Display(1,16,0);
		break;
		case Mode10://8H��autoģʽ�ж�Ϊҹ��8Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,8,0);
			SIG_Display(1,16,0);
		break;
		case Mode11://9H��autoģʽ�ж�Ϊҹ��9Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,21,0);
			SIG_Display(2,9,0);
			SIG_Display(1,16,0);
		break;
		case Mode12://10H��autoģʽ�ж�Ϊҹ��10Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,1,0);
			SIG_Display(2,0,0);
			SIG_Display(1,16,0);
		break;
		case Mode13://11H��autoģʽ�ж�Ϊҹ��11Сʱ��ػ���
			SIG_Display(4,21,0);
			SIG_Display(3,1,0);
			SIG_Display(2,1,0);
			SIG_Display(1,16,0);
		break;
		case Mode14://12H��autoģʽ�ж�Ϊҹ��12Сʱ��ػ���
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
	//����4
	if(param4 != 0)
		SIG_Display(4,param4,0);
	else
		SIG_Display(4,21,0);
	//����3ʱ�䵹����ʼʱ����Ҫ��˸
	if(TimeMsg.State == 1)
	{
		if(DocFlickerFlag == 0x00)
			SIG_Display(3,param3,1);
		else
			SIG_Display(3,param3,0);
	}
	else//����û��ʱ����˸
		SIG_Display(3,param3,0);
	//����2
	SIG_Display(2,param2,0);
	//����1
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
		if(TimeMsg.Tick++ >= 599940)//9999*60   1����
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
Describe: ������ʱ���
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void IrDelayDecet(void)
{
	static uint32_t count = 0;
	
	int8_t state = 0;
	
	state = READ_IR;//�õ�IR״̬
	if(EliminateState != state)
	{
		EliminateState = state;
		count = 0;
	}
	else
	{
		if(count++ == 300000)//100us һ���жϣ�30S = 30*1000000us
		{
			count = 300000;
			IrState = state;
		}
	}
}

/*---------------------------------------------------------------------
FuncName: DisplayFunc
Describe: ��ʾ����
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
	else if(SysMode == Mode1)//��ʾΪ ON
	{
		SIG_Display(4,21,0);
		SIG_Display(3,21,0);
		SIG_Display(2,0,0);
		SIG_Display(1,18,0);
		RELAY_HIGH;
	}
	else if(SysMode == Mode2)
	{//��ʾΪ AU���ж�Ϊҹ���������ж�Ϊ���죬���أ�
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
			case 0://0����⵽��ҹ������ʱ�䵹��ʱ
				if(IrState == 0)
				{
					TimeMsg.State = 1;//������ʱ
					RELAY_HIGH;
					TimingStep = 1;
				}
				else if(IrState == 1)
				{
					RELAY_LOW;
				}
			break;
			
			case 1://ʱ�䵽�ˣ��رռ̵���
				if(TimeMsg.Totaltime == 0)//��ʱ�䵽��󣬹رռ̵��� 
				{
					RELAY_LOW;
					TimeMsg.State = 0;//�رռ�ʱ��־
					TimeMsg.Tick = 0;//��0 tick
					
					if(IrState == 0)//���Ǻ�ҹ����ʱӦ����ʾ000
					{
						//��������
					}
					else if(IrState == 1)//���죬�����ݽ��лָ�
					{
						//TimeMsg.Totaltime//�ָ���ʱ��
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
		//����ʱ��ʾ
		CountdownTimeDispaly(TimeMsg.Totaltime);
	}
	
}

