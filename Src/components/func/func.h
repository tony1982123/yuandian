#ifndef __FUNC_H_
#define __FUNC_H_
#include "main.h"

typedef enum
{
	Mode0 = 0,//1.显示为OFF
	Mode1,//显示为 ON
	Mode2,//显示为 AU（判断为夜晚，常开，判断为白天，常关）
	Mode3,//1H（auto模式判断为夜晚，1小时后关机，auto）
	Mode4,//2H（auto模式判断为夜晚，2小时后关机）
	Mode5,//3H（auto模式判断为夜晚，3小时后关机）
	Mode6,//4H（auto模式判断为夜晚，4小时后关机）
	Mode7,//5H（auto模式判断为夜晚，5小时后关机）
	Mode8,//6H（auto模式判断为夜晚，6小时后关机）
	Mode9,//7H（auto模式判断为夜晚，7小时后关机）
	Mode10,//8H（auto模式判断为夜晚，8小时后关机）
	Mode11,//9H（auto模式判断为夜晚，9小时后关机）
	Mode12,//10H（auto模式判断为夜晚，10小时后关机）
	Mode13,//11H（auto模式判断为夜晚，11小时后关机）
	Mode14,//12H（auto模式判断为夜晚，12小时后关机）
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


//时间信息
typedef struct
{
	uint8_t 	State;//启动标识
	uint32_t	Tick;  //时间基准
	uint16_t    Tick_1s;  //时间基准
	uint16_t	Totaltime;//总时长
}TimeMsgType;

typedef struct
{
	uint8_t 	State; //状态标识
	uint32_t	Tick;  //时间基准
}IRMsgType;


void ModeAdjust(void);
void EnableMode(void);
void SysStateChangeHand(void);
void DisplayFunc(void);
void CountdownTimeHand(void);
void IrDelayDecet(void);
#endif
