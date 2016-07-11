#ifndef __RELAY_H__
#define __RELAY_H__

#include <stdio.h>
#include <stm32f10x.h>

/*
 @brief 继电器状态
 */
enum RELAY_TYPE {
	RELAY_ON, RELAY_OFF
};

enum RELAY_NO {
	RELAY_NO_0 = 0,
	RELAY_NO_1 = 1,
	RELAY_NO_2 = 2,
	RELAY_NO_3 = 3,
	RELAY_NO_4 = 4,
	RELAY_NO_5 = 5,
	RELAY_NO_6 = 6,
	RELAY_NO_7 = 7,
	RELAY_NO_8 = 8,
	RELAY_NO_9 = 9,
	RELAY_NO_10 = 10,
	RELAY_NO_11 = 11,
	RELAY_NO_12 = 12,
	RELAY_NO_13 = 13,
	RELAY_NO_14 = 14,
	RELAY_NO_15 = 15,
};

#define RELAY_MAX_SIZE 16

/*
 @brief 继电器定义
 */
struct RelayDefine {
	enum RELAY_NO relay_no;
	GPIO_TypeDef* pin_group;
	uint16_t pin;
	enum RELAY_TYPE defaultstatus;
};

/*
 @brief 设置继电器
 @param conf 继电器定义
 @param len 继电器长度
 */
extern uint8_t Relay_Configuration(struct RelayDefine conf[], int len);

/*
 @brief 继电器ON
 @param relay_no 继电器编号
 */
extern uint8_t Relay_ON(enum RELAY_NO relay_no);

/*
 @brief 继电器OFF
 @param relay_no 继电器编号
 */
extern uint8_t Relay_OFF(enum RELAY_NO relay_no);

#endif
