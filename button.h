#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_exti.h>
#include "boardconfig.h"

enum BUTTON_NO {
	BUTTON_NO_0 = 0,
	BUTTON_NO_1 = 1,
	BUTTON_NO_2 = 2,
	BUTTON_NO_3 = 3,
	BUTTON_NO_4 = 4,
	BUTTON_NO_5 = 5,
	BUTTON_NO_6 = 6,
	BUTTON_NO_7 = 7,
	BUTTON_NO_8 = 8,
	BUTTON_NO_9 = 9,
	BUTTON_NO_10 = 10,
	BUTTON_NO_11 = 11,
	BUTTON_NO_12 = 12,
	BUTTON_NO_13 = 13,
	BUTTON_NO_14 = 14,
	BUTTON_NO_15 = 15,
};
#define BUTTON_MAX_SIZE (16)

/*
 enum BUTTON_EVENT{
 BUTTON_PUSHED = 0
 };

 typedef void (*ButtonCallback)(enum BUTTON_NO btn_no, enum BUTTON_EVENT event);
 */

/*
 @brief 按钮定义
 */
struct ButtonDefine {
	enum BUTTON_NO btn_no;
	// PIN Group (GPIOA ...)
	GPIO_TypeDef* pin_group;
	// PIN (GPIO_Pin_1)
	uint16_t pin;
	// GPIO_PortSourceGPIOA ...
	uint8_t gpio_portsource;
	// GPIO_PinSource1 ...
	uint8_t gpio_pinsource;
	// EXTI_Line1 ...
	uint32_t exit_line_no;
	// EXTI1_IRQn ...
	IRQn_Type irqn;
};

/*
 @brief 按钮定义快速模板（只支持Pin0-Pin4的中断映射）
 */
#define BTNDEF_0_4(btnno, group, pin) {BUTTON_NO_##btnno, GPIO##group, GPIO_Pin_##pin, GPIO_PortSourceGPIO##group, GPIO_PinSource##pin, EXTI_Line##pin, EXTI##pin##_IRQn}

#define BTNDEF_5_9(btnno, group, pin) {BUTTON_NO_##btnno, GPIO##group, GPIO_Pin_##pin, GPIO_PortSourceGPIO##group, GPIO_PinSource##pin, EXTI_Line##pin, EXTI9_5_IRQn}

#define BTNDEF_10_15(btnno, group, pin) {BUTTON_NO_##btnno, GPIO##group, GPIO_Pin_##pin, GPIO_PortSourceGPIO##group, GPIO_PinSource##pin, EXTI_Line##pin, EXTI15_10_IRQn}

/*
 @brief 设置按钮
 @param 按钮定义
 @param 按钮定义数量
 */
extern uint8_t Button_Configuration(const struct ButtonDefine* defines,
		uint8_t len);

extern bool Button_IsPressed(enum BUTTON_NO btn_no);

#endif
