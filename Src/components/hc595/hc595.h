#ifndef __HC595_
#define __HC595_
#include "main.h"


#define HC595_SCK_SET	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET)//595Ê±ÖÓ
#define HC595_SCK_CLR	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)//(GPIO_ResetBits(GPIOB,GPIO_Pin_7))

#define HC595_RCK_SET	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)//595Ëø´æ
#define HC595_RCK_CLR	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)

#define HC595_SER_SET	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)	//595Êý¾Ý
#define	HC595_SER_CLR	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET)


#pragma   pack(1)


#pragma   pack()
void SIG_Display(uint8_t seg,uint8_t data ,uint8_t doc_flag);
void Hc595_Init(void);
void LED_TEST(void);
#endif
