#include "rcc.h"

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : ����ϵͳʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	/* ����ö�����ͱ��� HSEStartUpStatus */
	ErrorStatus HSEStartUpStatus;
	/* ��λϵͳʱ������ */
	RCC_DeInit();

	/* ʹ�ܲ�����ʱ��*/
	RCC_HSEConfig(RCC_HSE_ON);
	/*�ȴ�����*/
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		/*
		-----------------------------------
		|0<SYSCLK<24MHz   		|	0�ȴ�����	|
		|24MHz<SYSCLK<48MHz   |	1�ȴ�����	|
		|48MHz<SYSCLK<24MHz   |	2�ȴ�����	|
		-----------------------------------
		*/
		/* ����FLASH��ʱ������Ϊ1 */
		FLASH_SetLatency(FLASH_Latency_1);
		/* ʹ��FLASHԤȡ���� */
		FLASH_PrefetchBufferCmd(ENABLE);

		/* ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ =48M*/
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ PCLK2 = HCLK  = 48M*/
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
//		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET){}//�ȴ��ڲ��������
//		/*
//		-----------------------------------
//		|0<SYSCLK<24MHz   		|	0�ȴ�����	|
//		|24MHz<SYSCLK<48MHz   |	1�ȴ�����	|
//		|48MHz<SYSCLK<24MHz   |	2�ȴ�����	|
//		-----------------------------------
//		*/
//		/* ����FLASH��ʱ������Ϊ1 */
//		FLASH_SetLatency(FLASH_Latency_1);
//		/* ʹ��FLASHԤȡ���� */
//		FLASH_PrefetchBufferCmd(ENABLE);

//		/* ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ =48M*/
//		RCC_HCLKConfig(RCC_SYSCLK_Div1);
//		/* ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ PCLK2 = HCLK  = 48M*/
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

