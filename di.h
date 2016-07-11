#ifndef __DI_H__
#define __DI_H__

#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_exti.h>

/*
 @brief DI���
 */
enum DI_NO {
	DI_NO_0 = 0,
	DI_NO_1 = 1,
	DI_NO_2 = 2,
	DI_NO_3 = 3,
	DI_NO_4 = 4,
	DI_NO_5 = 5,
	DI_NO_6 = 6,
	DI_NO_7 = 7,
	DI_NO_8 = 8,
	DI_NO_9 = 9,
	DI_NO_10 = 10,
	DI_NO_11 = 11,
	DI_NO_12 = 12,
	DI_NO_13 = 13,
	DI_NO_14 = 14,
	DI_NO_15 = 15,
};

/*
 * @brief DI�������
 */
#define DI_MAX_SIZE 16

struct DiDefine {
	enum DI_NO di_no;
	GPIO_TypeDef* pin_group;
	uint16_t pin;
	uint8_t gpio_portsource;
	uint8_t gpio_pinsource;
	uint32_t exit_line_no;
	IRQn_Type irqn;
};

/*
 @brief ��ʼ���жϲ�����ֻ֧��Pin0-Pin4���ж�ӳ��, EXTI0 - EXTI4��
 */
#define  DIDEF_0_4(dino, group, pin) {DI_NO_##dino, GPIO##group, GPIO_Pin_##pin, GPIO_PortSourceGPIO##group, GPIO_PinSource##pin, EXTI_Line##pin, EXTI##pin##_IRQn}

/*
 @brief ��ʼ���жϲ�����ֻ֧��Pin5-Pin9���ж�ӳ�䣬EXTI9_5��
 */
#define  DIDEF_5_9(dino, group, pin) {DI_NO_##dino, GPIO##group, GPIO_Pin_##pin, GPIO_PortSourceGPIO##group, GPIO_PinSource##pin, EXTI_Line##pin, EXTI9_5_IRQn}

/*
 @brief DI����
 @param conf DI����
 @param len DI����
 */
extern uint8_t Di_Configuration(struct DiDefine conf[], int len);

/*
 @brief DI�Ƿ�������
 @param di_no DI�ı��
 */
extern bool Di_IsIN(enum DI_NO di_no);

#endif
