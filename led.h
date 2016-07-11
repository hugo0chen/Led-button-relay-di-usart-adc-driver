#ifndef __LED_H__
#define __LED_H__

#include <stdio.h>
#include <stm32f10x.h>
#include "boardconfig.h"

/*
 @brief LED��״̬
 */
enum LED_STATE {
	LEDON, LEDOFF
};

/*
 @brief LED���
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
 @brief LED���������
 */
#define LED_MAX_SIZE (16)

/*
 @brief LED����
 */
struct LedDefine {
	enum LED_NO led_no;
	// PIN_GROUP��GPIOA ...��
	GPIO_TypeDef* pin_group;
	// PIN��GPIO_Pin_1 ...��
	uint16_t pin;
	// LEDON | LEDOFF
	enum LED_STATE defaultstatus;
};

/*
 @brief ��ʼ������LED
 @param leds LED���ò���
 @param len LED������
 */
extern uint8_t LED_Configuration(const struct LedDefine* leds, uint8_t len);

/*
 @brief ����LED
 @param led_no LED���
 */
extern uint8_t LED_ON(enum LED_NO led_no);

/*
 @brief Ϩ��LED
 @param led_no LED���
 */
extern uint8_t LED_OFF(enum LED_NO led_no);

#endif
