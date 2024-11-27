#include "rcc.h"

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : 配置系统时钟
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	/* 定义枚举类型变量 HSEStartUpStatus */
	ErrorStatus HSEStartUpStatus;
	/* 复位系统时钟设置 */
	RCC_DeInit();

	/* 使能部高速时钟*/
	RCC_HSEConfig(RCC_HSE_ON);
	/*等待起振*/
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		/*
		-----------------------------------
		|0<SYSCLK<24MHz   		|	0等待周期	|
		|24MHz<SYSCLK<48MHz   |	1等待周期	|
		|48MHz<SYSCLK<24MHz   |	2等待周期	|
		-----------------------------------
		*/
		/* 设置FLASH延时周期数为1 */
		FLASH_SetLatency(FLASH_Latency_1);
		/* 使能FLASH预取缓存 */
		FLASH_PrefetchBufferCmd(ENABLE);

		/* 选择HCLK（AHB）时钟源为SYSCLK 1分频 =48M*/
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* 选择PCLK2时钟源为 HCLK（AHB） 1分频 PCLK2 = HCLK  = 48M*/
		RCC_PCLKConfig(RCC_HCLK_Div1);


		/* PLLCLK = 8MHz * 6 = 48 MHz */
		RCC_PREDIV1Config(RCC_PREDIV1_Div1);
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		 /* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		RCC_ClockSecuritySystemCmd(ENABLE);
		 /* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
//	else
//	{
//		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET){}//等待内部晶振就绪
//		/*
//		-----------------------------------
//		|0<SYSCLK<24MHz   		|	0等待周期	|
//		|24MHz<SYSCLK<48MHz   |	1等待周期	|
//		|48MHz<SYSCLK<24MHz   |	2等待周期	|
//		-----------------------------------
//		*/
//		/* 设置FLASH延时周期数为1 */
//		FLASH_SetLatency(FLASH_Latency_1);
//		/* 使能FLASH预取缓存 */
//		FLASH_PrefetchBufferCmd(ENABLE);

//		/* 选择HCLK（AHB）时钟源为SYSCLK 1分频 =48M*/
//		RCC_HCLKConfig(RCC_SYSCLK_Div1);
//		/* 选择PCLK2时钟源为 HCLK（AHB） 1分频 PCLK2 = HCLK  = 48M*/
//		RCC_PCLKConfig(RCC_HCLK_Div1);


//		/* PLLCLK = 8MHz * 6 = 48 MHz */
//		RCC_PREDIV1Config(RCC_PREDIV1_Div1);
//		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);

//		/* Enable PLL */
//		RCC_PLLCmd(ENABLE);

//		 /* Wait till PLL is ready */
//		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//		{
//		}

//		/* Select PLL as system clock source */
//		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

//		RCC_ClockSecuritySystemCmd(ENABLE);
//		 /* Wait till PLL is used as system clock source */
//		while(RCC_GetSYSCLKSource() != 0x08)
//		{
//		}
//	}
}

