#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"

#include "stm32f10x.h"

										  
void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void SPI1_Init(void);
 		 
#endif

