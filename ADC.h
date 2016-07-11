#ifndef __ADC_H
#define __ADC_H	

#include "stm32f10x_adc.h"

/*
 @brief ADC���
 */
enum ADC_NO {
	ADC_NO_0 = 0, ADC_NO_1 = 1, ADC_NO_2 = 2
};

/*
 @brief ���ADC�ӿ�����
 */
#define ADC_MAX_SIZE (3)

/*
 @brief ADC����ṹ��
 */
struct ADCDefine {
	// ADC ���
	enum ADC_NO adc_no;
	// ADC ���ͣ�ADC1 | ADC2 �ȣ�
	ADC_TypeDef * adc_type;
	// GPIO�飨GPIOA | GPIOB �ȣ�
	GPIO_TypeDef* pin_group;
	// GPIO�ţ�GPIO_Pin_1�ȣ�
	uint16_t pin;
};

/*
 @brief ����ADC
 @param defines ������Ҫ���õ�ADC��Ϣ
 @param len ����ADC������
 */
extern void ADC_Configuration(const struct ADCDefine* defines, uint8_t len);

/*
 @brief ��ȡADC��ֵ
 @param adc_no ADC��ţ�������ʱ��ͬ
 @param ch ADCͨ��
 */
extern u16 ADC_Get(enum ADC_NO adc_no, u8 ch);

/*
 @brief ��ȡ������ƽ��ֵ
 @param adc_no ADC���
 @param ch ADCͨ����
 @param times ��������
 */
extern float ADC_GetAverage(enum ADC_NO adc_no, u8 ch, u8 times);

#endif 
