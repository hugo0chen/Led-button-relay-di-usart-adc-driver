#include "GlobalVar.h"
#include "usart.h"

unsigned char receiveBuf[8];
unsigned char receiveCount;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/************************** new *****************************/

USART_RECV_CALLBACK _usart_recv_callbacks[4] = { 0 };
struct USARTto485Define _usart_485_defines[4] = { 0 };

#define SetRs485EnPort(usart_no)  GPIO_SetBits(_usart_485_defines[usart_no].pin_of_tre_group, _usart_485_defines[usart_no].pin_of_tre)
#define ClrRs485EnPort(usart_no)  GPIO_ResetBits(_usart_485_defines[usart_no].pin_of_tre_group, _usart_485_defines[usart_no].pin_of_tre)

#define UartIsConfigurated(rs485_no) (_usart_485_defines[rs485_no].usartdef!=NULL)
#define UartTypeDef(rs485_no) (_usart_485_defines[rs485_no].usartdef)

void _usart1_config_to_rs485(struct USARTto485Define config) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_tre;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(config.pin_of_tre_group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_txd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(config.pin_of_txd_group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_rxd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(config.pin_of_rxd_group, &GPIO_InitStructure);

	USART_Init(USART1, &config.rs485_param);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

	SetRs485EnPort(config.rs485_no);
}

void _usart2_config_to_rs485(struct USARTto485Define config) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_tre;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(config.pin_of_tre_group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_txd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(config.pin_of_txd_group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_rxd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(config.pin_of_rxd_group, &GPIO_InitStructure);

	USART_Init(USART2, &config.rs485_param);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

	SetRs485EnPort(config.rs485_no);
}

void _usart3_config_to_rs485(struct USARTto485Define config) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_tre;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(config.pin_of_tre_group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_txd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(config.pin_of_txd_group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = config.pin_of_rxd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(config.pin_of_rxd_group, &GPIO_InitStructure);

	USART_Init(USART3, &config.rs485_param);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);

	SetRs485EnPort(config.rs485_no);
}

void _usart_nvic_config(uint32_t usart_irqn, uint32_t preemptionpriority,
		uint32_t subpriority) {
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = usart_irqn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void _usart_comm_irqhandler(USART_TypeDef* usarttypedef,
		enum BOARD_USART_TYPE usart_type) {
	unsigned char recv_char;
	if (USART_GetFlagStatus(usarttypedef, USART_FLAG_RXNE) != RESET) {
		USART_ClearITPendingBit(usarttypedef, USART_IT_RXNE);
		recv_char = USART_ReceiveData(usarttypedef);
		if (_usart_recv_callbacks[usart_type] != NULL)
			_usart_recv_callbacks[usart_type](usart_type, recv_char);
	}
}

void USART1_IRQHandler(void) {
	_usart_comm_irqhandler(USART1, BOARD_USART_1);
}

void USART2_IRQHandler(void) {
	_usart_comm_irqhandler(USART2, BOARD_USART_2);
}

void USART3_IRQHandler(void) {
	_usart_comm_irqhandler(USART3, BOARD_USART_3);
}

void _usart_sendchar(USART_TypeDef* usart_typedef, uint8_t sChar) {
	// Write a character to the USART
	USART_SendData(usart_typedef, (uint8_t) sChar);
	while (USART_GetFlagStatus(usart_typedef, USART_FLAG_TXE) == RESET) {
	}
}

uint32_t USART_SendString(enum RS485_NO rs485_no, const char* str,
		uint8_t strlen) {
	uint8_t i;
	uint16_t data;
	if (UartIsConfigurated(rs485_no)) {
		USART_ITConfig(UartTypeDef(rs485_no), USART_IT_RXNE, DISABLE);
		SetRs485EnPort(rs485_no);
		printf("485 send %s len = %u\r\n", str, strlen);
		for (i = 0; i < strlen; i++) {
			data = *((uint8_t*) str++);
			printf("usart send <485:%d>: %x\r\n", rs485_no, data);
			//_usart_sendchar( UartTypeDef(usart_type), );
			USART_SendData(UartTypeDef(rs485_no), data);
		}
		delay_us(10000);
		ClrRs485EnPort(rs485_no);
		// USART_ReceiveData(UartTypeDef(rs485_no));
		USART_ITConfig(UartTypeDef(rs485_no), USART_IT_RXNE, ENABLE);
	}
	return 0;
}

uint32_t USART_Setting485(enum RS485_NO rs485_no, USART_InitTypeDef param) {
	USART_TypeDef* usartdef = UartTypeDef(rs485_no);

	_usart_485_defines[rs485_no].rs485_param = param;

	USART_ITConfig(usartdef, USART_IT_RXNE, DISABLE);
	USART_Cmd(usartdef, DISABLE);

	USART_Init(usartdef, &param);
	USART_Cmd(usartdef, ENABLE);
	SetRs485EnPort(rs485_no);
	USART_ITConfig(usartdef, USART_IT_RXNE, ENABLE);
	return 0;
}

const struct USARTto485Define* USART_Get485Define(enum RS485_NO rs485_no) {
	return &_usart_485_defines[rs485_no];
}

PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(UART5, (u8) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
		;
	return ch;
}

void _usart_rcc_config(uint8_t apb1_enable, uint8_t apb2_enable) {
	if (apb2_enable > 0) {
		/*
		 只有USART1在APB2上
		 */
		RCC_APB2PeriphClockCmd(
				RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
						| RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
						| RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
	}
	if (apb1_enable > 0) {
		// TODO apb1_enable
	}
}

uint32_t USART_To485_Configuration(const struct USARTto485Define* configs,
		uint16_t len) {
	uint16_t u = 0;
	uint8_t apb1_enable = 0;
	uint8_t apb2_enable = 0;

	for (u = 0; u < len; ++u) {
		struct USARTto485Define config = configs[u];
		_usart_485_defines[config.rs485_no] = config;
		if (config.usartdef == USART1) {
			_usart1_config_to_rs485(config);
			_usart_nvic_config(USART1_IRQn, config.preemption_priority, 0);
			++apb2_enable; // USART1 在APB2上
			_usart_recv_callbacks[BOARD_USART_1] = config.recvcallback;

		} else if (config.usartdef == USART2) {
			_usart2_config_to_rs485(config);
			_usart_nvic_config(USART2_IRQn, config.preemption_priority, 0);
			++apb1_enable; // USART2 在APB1上
			_usart_recv_callbacks[BOARD_USART_2] = config.recvcallback;

		} else if (config.usartdef == USART3) {
			_usart3_config_to_rs485(config);
			_usart_nvic_config(USART3_IRQn, config.preemption_priority, 0);
			++apb1_enable; // USART3 在APB1上
			_usart_recv_callbacks[BOARD_USART_3] = config.recvcallback;
		} else {
			return 1;
		}
	}
	return 0;
}

uint32_t USART_Configuration(const struct USARTDefine* configs, uint16_t len) {
	//NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART5, &USART_InitStructure);
	//USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5, ENABLE);

	/*NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);*/

	return 0;
}

