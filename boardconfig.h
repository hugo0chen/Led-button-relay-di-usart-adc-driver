#ifndef __BOARDCONFIG_H__
#define __BOARDCONFIG_H__

#include <stdio.h>
#include <stm32f10x.h>

/*
 To define board global configuration
 */
#define BOARD_NVIC_PRIORITYGROUP NVIC_PriorityGroup_1

/*
 @brief ��ʼ��NVIC��ȫ������
 @param priority_group �ն�������
 */
extern uint16_t BOARD_NVIC_Init(uint16_t priority_group);

#define NVIC_INIT() NVIC_Configuration(BOARD_NVIC_PRIORITYGROUP)

#endif
