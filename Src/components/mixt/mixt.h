#ifndef __MIXT_H_
#define __MIXT_H_
#include "main.h"
#include "py32f002b_hal_gpio.h"


#define RELAY_HIGH	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
#define RELAY_LOW	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#define READ_IR    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)

void mixt_init(void);
#endif

