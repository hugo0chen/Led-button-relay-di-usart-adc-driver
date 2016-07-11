#ifndef __ADC_H
#define __ADC_H	

#include "stm32f10x_adc.h"

/*
 @brief ADC编号
 */
enum ADC_NO {
	ADC_NO_0 = 0, ADC_NO_1 = 1, ADC_NO_2 = 2
};

/*
 @brief 最大ADC接口数量
 */
#define ADC_MAX_SIZE (3)

/*
 @brief ADC定义结构体
 */
struct ADCDefine {
	// ADC 编号
	enum ADC_NO adc_no;
	// ADC 类型（ADC1 | ADC2 等）
	ADC_TypeDef * adc_type;
	// GPIO组（GPIOA | GPIOB 等）
	GPIO_TypeDef* pin_group;
	// GPIO脚（GPIO_Pin_1等）
	uint16_t pin;
};

/*
 @brief 设置ADC
 @param defines 定义需要设置的ADC信息
 @param len 定义ADC的数量
 */
extern void ADC_Configuration(const struct ADCDefine* defines, uint8_t len);

/*
 @brief 获取ADC的值
 @param adc_no ADC编号，与设置时相同
 @param ch ADC通道
 */
extern u16 ADC_Get(enum ADC_NO adc_no, u8 ch);

/*
 @brief 获取次数的平均值
 @param adc_no ADC编号
 @param ch ADC通道号
 @param times 采样次数
 */
extern float ADC_GetAverage(enum ADC_NO adc_no, u8 ch, u8 times);

#endif 
