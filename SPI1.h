#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"

#include "stm32f10x.h"

										  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void SPI1_Init(void);
 		 
#endif

