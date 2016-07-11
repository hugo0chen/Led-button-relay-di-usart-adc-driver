#ifndef __SHT10_H__
#define __SHT10_H__
#include "stm32f10x.h"

enum {
	TEMP, 
    HUMI
};

enum SHT10_NO {
	SHT10_NO_0 = 0, SHT10_NO_1 = 1, SHT10_NO_2 = 2, SHT10_NO_3 = 3
};
#define SHT10_MAX_SIZE (4)

struct SHT10Define {
	enum SHT10_NO sht10_no;
	GPIO_TypeDef* pin_of_data_group;
	uint16_t pin_of_data;
	GPIO_TypeDef* pin_of_sck_group;
	uint16_t pin_of_sck;
};

void SHT10_Configuration(struct SHT10Define* defines, uint8_t len);

/*
 @brief 重置通讯
 */
void SHT10_ConReset(enum SHT10_NO sht10_no);

/*
 @brief 软复位
 */
u8 SHT10_SoftReset(enum SHT10_NO sht10_no);

u8 SHT10_Measure(enum SHT10_NO sht10_no, u16 *p_value, u8 *p_checksum, u8 mode);
void SHT10_Calculate(enum SHT10_NO sht10_no, u16 t, u16 rh,
		float *p_temperature, float *p_humidity);
float SHT10_CalcuDewPoint(enum SHT10_NO sht10_no, float t, float h);

#endif

