#ifndef __USERFLASH_H
#define __USERFLASH_H
#include "main.h"

//FLASH起始地址
#define 	STM32_FLASH_BASE ((uint32_t)0x08000000)  	//STM32 FLASH起始地址


//FLASH扇区起始地址
#define ADDR_FLASH_PAGE0     ((uint32_t)0x08000000) 			//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE1     ((uint32_t)0x08000000) +1*1024		//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE2     ((uint32_t)0x08000000) +2*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE3     ((uint32_t)0x08000000) +3*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE4     ((uint32_t)0x08000000) +4*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE5     ((uint32_t)0x08000000) +5*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE6     ((uint32_t)0x08000000) +6*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE7     ((uint32_t)0x08000000) +7*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE8     ((uint32_t)0x08000000) +8*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE9     ((uint32_t)0x08000000) +9*1024		//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE10    ((uint32_t)0x08000000) +10*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE11    ((uint32_t)0x08000000) +11*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE12    ((uint32_t)0x08000000) +12*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE13    ((uint32_t)0x08000000) +13*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE14    ((uint32_t)0x08000000) +14*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE15    ((uint32_t)0x08000000) +15*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE16    ((uint32_t)0x08000000) +16*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE17    ((uint32_t)0x08000000) +17*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE18    ((uint32_t)0x08000000) +18*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE19    ((uint32_t)0x08000000) +19*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE20    ((uint32_t)0x08000000) +20*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE21    ((uint32_t)0x08000000) +21*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE22    ((uint32_t)0x08000000) +22*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE23    ((uint32_t)0x08000000) +23*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
/*
#define ADDR_FLASH_PAGE24    ((uint32_t)0x08000000) +24*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE25    ((uint32_t)0x08000000) +26*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE26    ((uint32_t)0x08000000) +26*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE27    ((uint32_t)0x08000000) +27*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE28    ((uint32_t)0x08000000) +28*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE29    ((uint32_t)0x08000000) +29*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE30    ((uint32_t)0x08000000) +30*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
#define ADDR_FLASH_PAGE31    ((uint32_t)0x08000000) +31*1024	//扇区0, 1 Kbytes  	//扇区0, 1 Kbytes  
*/


void STM32SystemReset(void);
uint32_t GetSector(uint32_t  addr);


void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);
void STMFLASH_Write_HalfWord(uint32_t WriteAddr,uint16_t *pBuffer,uint32_t NumToWrite)	;
uint8_t EraseSector(uint32_t FLASH_Sector);
uint32_t ReadByte_FLASH(uint32_t Address);
void ReadData_FLASH(uint32_t start_addr,uint8_t *data,uint16_t size);
uint8_t sys_data_write(uint32_t start_addr,uint16_t *p_data,uint32_t size);
void flash_RWprotection(void);


#endif
