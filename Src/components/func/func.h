#ifndef __FUNC_H_
#define __FUNC_H_
#include "main.h"

typedef enum
{
	Mode0 = 0,//1.��ʾΪOFF
	Mode1,//��ʾΪ ON
	Mode2,//��ʾΪ AU���ж�Ϊҹ���������ж�Ϊ���죬���أ�
	Mode3,//1H��autoģʽ�ж�Ϊҹ��1Сʱ��ػ���auto��
	Mode4,//2H��autoģʽ�ж�Ϊҹ��2Сʱ��ػ���
	Mode5,//3H��autoģʽ�ж�Ϊҹ��3Сʱ��ػ���
	Mode6,//4H��autoģʽ�ж�Ϊҹ��4Сʱ��ػ���
	Mode7,//5H��autoģʽ�ж�Ϊҹ��5Сʱ��ػ���
	Mode8,//6H��autoģʽ�ж�Ϊҹ��6Сʱ��ػ���
	Mode9,//7H��autoģʽ�ж�Ϊҹ��7Сʱ��ػ���
	Mode10,//8H��autoģʽ�ж�Ϊҹ��8Сʱ��ػ���
	Mode11,//9H��autoģʽ�ж�Ϊҹ��9Сʱ��ػ���
	Mode12,//10H��autoģʽ�ж�Ϊҹ��10Сʱ��ػ���
	Mode13,//11H��autoģʽ�ж�Ϊҹ��11Сʱ��ػ���
	Mode14,//12H��autoģʽ�ж�Ϊҹ��12Сʱ��ػ���
	ModeMax = 255,
}SysModeEnum;

#define TIME_1H 	60
#define TIME_2H 	120
#define TIME_3H 	180
#define TIME_4H 	240
#define TIME_5H 	300
#define TIME_6H 	360
#define TIME_7H 	420
#define TIME_8H 	480
#define TIME_9H 	540
#define TIME_10H 	600
#define TIME_11H 	660
#define TIME_12H 	720

typedef enum
{
	AdjustState = 0,
	DisplayState ,
}SysStateEnum;


//ʱ����Ϣ
typedef struct
{
	uint8_t 	State;//������ʶ
	uint32_t	Tick;  //ʱ���׼
	uint16_t    Tick_1s;  //ʱ���׼
	uint16_t	Totaltime;//��ʱ��
}TimeMsgType;

typedef struct
{
	uint8_t 	State; //״̬��ʶ
	uint32_t	Tick;  //ʱ���׼
}IRMsgType;


void ModeAdjust(void);
void EnableMode(void);
void SysStateChangeHand(void);
void DisplayFunc(void);
void CountdownTimeHand(void);
void IrDelayDecet(void);
#endif
