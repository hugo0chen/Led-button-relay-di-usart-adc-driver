#ifndef __RELAY_H__
#define __RELAY_H__

#include <stdio.h>
#include <stm32f10x.h>

/*
 @brief �̵���״̬
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
 @brief �̵�������
 */
struct RelayDefine {
	enum RELAY_NO relay_no;
	GPIO_TypeDef* pin_group;
	uint16_t pin;
	enum RELAY_TYPE defaultstatus;
};

/*
 @brief ���ü̵���
 @param conf �̵�������
 @param len �̵�������
 */
extern uint8_t Relay_Configuration(struct RelayDefine conf[], int len);

/*
 @brief �̵���ON
 @param relay_no �̵������
 */
extern uint8_t Relay_ON(enum RELAY_NO relay_no);

/*
 @brief �̵���OFF
 @param relay_no �̵������
 */
extern uint8_t Relay_OFF(enum RELAY_NO relay_no);

#endif
