#include "Userflash.h"
#include "USART.h"

#define FLASH_Lock   HAL_FLASH_Lock 
#define FLASH_Unlock HAL_FLASH_Unlock 

/*******************************************************************************
* Function Name  : STM32SystemReset
* Description    : STM32????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STM32SystemReset(void)
{
	//__set_FAULTMASK(1);//
	NVIC_SystemReset();
	
}



/*******************************************************************************
* Function Name  : stm32f0flash_GetFlashSector
* Description    : 获取某个地址所在的扇区
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t GetSector(uint32_t  addr)
{
	if(addr<ADDR_FLASH_PAGE1)return ADDR_FLASH_PAGE0;
	else if(addr<ADDR_FLASH_PAGE2)return ADDR_FLASH_PAGE1;
	else if(addr<ADDR_FLASH_PAGE3)return ADDR_FLASH_PAGE2;
	else if(addr<ADDR_FLASH_PAGE4)return ADDR_FLASH_PAGE3;
	else if(addr<ADDR_FLASH_PAGE5)return ADDR_FLASH_PAGE4;
	else if(addr<ADDR_FLASH_PAGE6)return ADDR_FLASH_PAGE5;
	else if(addr<ADDR_FLASH_PAGE4)return ADDR_FLASH_PAGE6;
	else if(addr<ADDR_FLASH_PAGE8)return ADDR_FLASH_PAGE7;
	else if(addr<ADDR_FLASH_PAGE9)return ADDR_FLASH_PAGE8;
	else if(addr<ADDR_FLASH_PAGE10)return ADDR_FLASH_PAGE9;
	else if(addr<ADDR_FLASH_PAGE11)return ADDR_FLASH_PAGE10;
	else if(addr<ADDR_FLASH_PAGE12)return ADDR_FLASH_PAGE11;
	else if(addr<ADDR_FLASH_PAGE13)return ADDR_FLASH_PAGE12;
	else if(addr<ADDR_FLASH_PAGE14)return ADDR_FLASH_PAGE13;
	else if(addr<ADDR_FLASH_PAGE15)return ADDR_FLASH_PAGE14;
	else if(addr<ADDR_FLASH_PAGE16)return ADDR_FLASH_PAGE15;
	else if(addr<ADDR_FLASH_PAGE17)return ADDR_FLASH_PAGE16;
	else if(addr<ADDR_FLASH_PAGE18)return ADDR_FLASH_PAGE17;
	else if(addr<ADDR_FLASH_PAGE19)return ADDR_FLASH_PAGE18;
	else if(addr<ADDR_FLASH_PAGE20)return ADDR_FLASH_PAGE19;
	else if(addr<ADDR_FLASH_PAGE21)return ADDR_FLASH_PAGE20;
	else if(addr<ADDR_FLASH_PAGE22)return ADDR_FLASH_PAGE21;
	else if(addr<ADDR_FLASH_PAGE23)return ADDR_FLASH_PAGE22;
/*	else if(addr<ADDR_FLASH_PAGE24)return ADDR_FLASH_PAGE23;
	else if(addr<ADDR_FLASH_PAGE25)return ADDR_FLASH_PAGE24;
	else if(addr<ADDR_FLASH_PAGE26)return ADDR_FLASH_PAGE25;
	else if(addr<ADDR_FLASH_PAGE27)return ADDR_FLASH_PAGE26;
	else if(addr<ADDR_FLASH_PAGE28)return ADDR_FLASH_PAGE27;
	else if(addr<ADDR_FLASH_PAGE29)return ADDR_FLASH_PAGE28;
	else if(addr<ADDR_FLASH_PAGE30)return ADDR_FLASH_PAGE29;
	else if(addr<ADDR_FLASH_PAGE31)return ADDR_FLASH_PAGE30;*/
	return ADDR_FLASH_PAGE23;	
}

uint8_t sys_data_write(uint32_t start_addr,uint16_t *p_data,uint32_t size)
{
	uint32_t PAGEError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct = {0};
	uint32_t end_addr=start_addr+size*2;
	uint32_t address=start_addr;
	int32_t i=0;
	uint8_t memory_pro_status=0;

	if(start_addr<STM32_FLASH_BASE)
	{
		return 1;	
	}

	FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGEERASE;                                     /* Page Erase */
  EraseInitStruct.PageAddress = start_addr;                                         /* Erase Start Address */
  EraseInitStruct.NbPages  = size*2 / FLASH_PAGE_SIZE; 
	if(HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		printf("PAGEError = %d \r\n",PAGEError);
		memory_pro_status=1;
	}

	//write flash
	while((memory_pro_status!=1)&&(address<end_addr))
	{
		//flash_status=FLASH_ProgramHalfWord(address,p_data[i++]);
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, address, (uint32_t *)&p_data[i]) == HAL_OK)
		{
			address += FLASH_PAGE_SIZE;                                           /* flash Start point first page */
      i += FLASH_PAGE_SIZE / 4; 
		}
		else
		{
			memory_pro_status=1;
		}
		address=address+FLASH_PAGE_SIZE / 4;
	}

	
	address=start_addr;
	i=0;
	while((address<end_addr)&&(memory_pro_status!=1))
	{
		if((*(uint16_t*)address)!=p_data[i++])
		{
			memory_pro_status=1;
			printf("flash veryfy err!!!\r\n");
		}
		address+=2;
	}
	
	FLASH_Lock();

	return memory_pro_status;
}


/*******************************************************************************
* Function Name  : ReadData_FLASH
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReadData_FLASH(uint32_t start_addr,uint8_t *data,uint16_t size)
{
	uint16_t i;
	
	for(i=0;i<size;i++)
	{
		*data=*(uint32_t*)start_addr;
		data++;
		start_addr++;
	}	
}


