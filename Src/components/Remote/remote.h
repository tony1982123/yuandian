#ifndef __REMOTE_H_
#define __REMOTE_H_
#include "main.h"
#define RemoteUp 	0xFC5555
#define RemoteOk 	0xF35555
#define RemoteDown 	0xCF5555



#define RF_Dat HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)


void Remote_Init(void);
void get_ir_state(void);
#endif
