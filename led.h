#ifndef __LED_H__
#define __LED_H__

#include <stdio.h>
#include <stm32f10x.h>
#include "boardconfig.h"

/*
 @brief LED的状态
 */
enum LED_STATE {
	LEDON, LEDOFF
};

/*
 @brief LED编号
 */
enum LED_NO {
	LED_NO_0 = 0,
	LED_NO_1 = 1,
	LED_NO_2 = 2,
	LED_NO_3 = 3,
	LED_NO_4 = 4,
	LED_NO_5 = 5,
	LED_NO_6 = 6,
	LED_NO_7 = 7,
	LED_NO_8 = 8,
	LED_NO_9 = 9,
	LED_NO_10 = 10,
	LED_NO_11 = 11,
	LED_NO_12 = 12,
	LED_NO_13 = 13,
	LED_NO_14 = 14,
	LED_NO_15 = 15
};

/*
 @brief LED的最大数量
 */
#define LED_MAX_SIZE (16)

/*
 @brief LED定义
 */
struct LedDefine {
	enum LED_NO led_no;
	// PIN_GROUP（GPIOA ...）
	GPIO_TypeDef* pin_group;
	// PIN（GPIO_Pin_1 ...）
	uint16_t pin;
	// LEDON | LEDOFF
	enum LED_STATE defaultstatus;
};

/*
 @brief 初始化所有LED
 @param leds LED设置参数
 @param len LED的数量
 */
extern uint8_t LED_Configuration(const struct LedDefine* leds, uint8_t len);

/*
 @brief 点亮LED
 @param led_no LED编号
 */
extern uint8_t LED_ON(enum LED_NO led_no);

/*
 @brief 熄灭LED
 @param led_no LED编号
 */
extern uint8_t LED_OFF(enum LED_NO led_no);

#endif
