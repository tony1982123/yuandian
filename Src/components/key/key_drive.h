#ifndef _KEY_DRIVE_H_
#define _KEY_DRIVE_H_

#include "main.h"

//上键
#define KEY_UP_PIN  		GPIO_PIN_4
#define KEY_UP_PORT 		GPIOA

//下键
#define KEY_DOWN_PIN 		GPIO_PIN_5
#define KEY_DOWN_PORT  		GPIOA

//OK
#define KEY_OK_PIN 			GPIO_PIN_6
#define KEY_OK_PORT  		GPIOA



//定时器10ms扫描一次  单位要*10
#define SINGLE_TIME  1000     //按下30ms后再按下即为一次单击，30ms内均为无效
#define DOUBLE_TIME  3000    //300ms内接收到两次单击即为双击，双击后300ms再单击为三击
#define LONG_KEY_TIME  100000 //长按限制时间2000ms



/* 按键状态机 */
#define KEY_NO_ACTION            0      //没有按键按下
#define KEY_CLICK_ACTION         1      //单击
#define KEY_DOUBLE_ACTION        2      //双击
#define KEY_LONG_PRESS_ACTION    3      //长按
#define KEY_LONG_RELEASE_ACTION  4      //长按释放



#define key_state_0   0  //状态0
#define key_state_1   1  //状态1
#define key_state_2   2  //状态2
#define key_state_3   3  //状态3

struct KeyInfo
{
	uint16_t usr_timer_cnt;   //按键按下计数器
	uint16_t key_timer_cnt;   //双击计数器
	uint8_t  press_state;     //按键按下与释放状态
	uint16_t key_times;  	   //按键按住时间计数值
	uint8_t  key_flag;   	   //按键按下标志位
	uint8_t  repeat_state;    //按键重复状态（双击判断）
	uint8_t  click_flag; 	   //击打标识
};






uint8_t key_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,struct KeyInfo *info);
void KeyTimeGroup(void); 
void KEY_Init(void);

#endif
