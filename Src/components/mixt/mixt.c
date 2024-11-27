#include "mixt.h"
#include "delay.h"
#include "Userflash.h"
#include "func.h"
extern SysModeEnum SysMode;


void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR ^= GPIO_Pin;
}


static void IR_Init(void)
{	
	GPIO_InitTypeDef GpioInitStruct;
	
	GpioInitStruct.Pin = GPIO_PIN_7;
	GpioInitStruct.Mode = GPIO_MODE_INPUT;
	GpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	//GpioInitStruct.GPIO_OType = GPIO_OType_PP;
	GpioInitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GpioInitStruct);
}

static void relay_Init(void)
{	
	GPIO_InitTypeDef GpioInitStruct;
	
	GpioInitStruct.Pin = GPIO_PIN_1;
	GpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	GpioInitStruct.GPIO_OType = GPIO_OType_PP;
	GpioInitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GpioInitStruct);
}

/*---------------------------------------------------------------------
FuncName: get_lifetime_val
Describe: 读取flash中的参数
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void mixt_init(void)
{
	ReadData_FLASH(ADDR_FLASH_PAGE15,(uint8_t*)&SysMode,1);
	if(SysMode == 0xFF)
	{
		SysMode = Mode0;
		sys_data_write(ADDR_FLASH_PAGE15,(uint16_t*)&SysMode,1);
	}
	IR_Init();
	relay_Init();
}







