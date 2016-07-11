#ifndef __USART_H__
#define __USART_H__

#include <stdio.h>
#include <stm32f10x.h>
#include "boardconfig.h"

/*
 * @brief 485编号
 */
enum RS485_NO {
	RS485_NULL = 0, RS485_NO_1 = 1, RS485_NO_2 = 2
};

/*
 * @brief USART总线编号
 */
enum BOARD_USART_TYPE {
	BOARD_USART_1 = 1, BOARD_USART_2 = 2, BOARD_USART_3 = 3
};

typedef int (*USART_RECV_CALLBACK)(enum BOARD_USART_TYPE usart_no,
		unsigned char recv);

struct USARTto485Define {
	enum RS485_NO rs485_no;
	USART_TypeDef* usartdef;
	uint16_t preemption_priority;
	GPIO_TypeDef* pin_of_tre_group;
	uint16_t pin_of_tre;
	/* RX本端读 */
	GPIO_TypeDef* pin_of_rxd_group;
	uint16_t pin_of_rxd;
	/* TX本端写 */
	GPIO_TypeDef* pin_of_txd_group;
	uint16_t pin_of_txd;
	USART_InitTypeDef rs485_param;
	USART_RECV_CALLBACK recvcallback;
};

struct USARTDefine {
	GPIO_TypeDef* pin_of_txd_group;
	uint16_t pin_of_txd;
	GPIO_TypeDef* pin_of_rxd_group;
	uint16_t pin_of_rxd;
	uint32_t bandrate;
	USART_RECV_CALLBACK recvcallback;
};

/*
 @brief 使用USART初始化485接口
 @param config 配置数组
 {
 { RS485_NO_1, USART1, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, 9600, xxx},{}
 }
 @param len 初始化的数量
 @return 设置结果(0=SUCCESS, 1=FAILED)
 @desc 需要先设置NVIC (@ref BOARD_NVIC_Init) 和RCC
 */
extern uint32_t USART_To485_Configuration(
		const struct USARTto485Define* configs, uint16_t len);

/*
 @brief 发送数据
 @param usart_type USART类型
 @param str 字符串指针
 @param strlen 字符串长度
 */
extern uint32_t USART_SendString(enum RS485_NO rs485_no, const char* str,
		uint8_t strlen);

/*
 @brief 使用UART初始化接口
 @param config 配置数组
 @param len 初始化的数量
 @return 设置结果(0=SUCCESS, 1=FAILED)
 @desc 需要先设置NVIC (@ref BOARD_NVIC_Init) 和RCC
 */
extern uint32_t USART_Configuration(const struct USARTDefine* configs,
		uint16_t len);

/*
 @brief 设置485参数
 @param rs485_no 485编号
 @param param 参数
 */
extern uint32_t USART_Setting485(enum RS485_NO rs485_no,
		USART_InitTypeDef param);

extern const struct USARTto485Define* USART_Get485Define(
		enum RS485_NO rs485_no);

#endif
