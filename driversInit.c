#include <stm32f10x.h>
#include "usart.h"
#include "led.h"
#include "Delay.h"
#include "LCD.h"
#include "ADC.h"
#include "SPI1.h"
#include "SHT10.h"
#include "netconf.h"
#include "stm32_eth.h"
#include "Ethernet.h"
#include "relay.h"
#include "di.h"
#include "button.h"
#include "GlobalVar.h"
#include "driversInit.h"
#include "GlobalConstants.h"

void RCC_Configuration()
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );
}


void init_gpio_remap()
{
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable, ENABLE );
	GPIO_PinRemapConfig( GPIO_Remap_SPI1, ENABLE );
}

void init_rs485()
{
	/*
	 * struct USARTto485Define{
	 *   enum RS485_NO rs485_no;
	 *   enum USART_TypeDef* usartdef;
	 *   uint16_t preemption_priority;
	 *   GPIO_TypeDef* pin_of_tre_group;
	 *   uint16_t pin_of_tre;
	 *   GPIO_TypeDef* pin_of_txd_group;
	 *   uint16_t pin_of_txd;
	 *   GPIO_TypeDef* pin_of_rxd_group;
	 *   uint16_t pin_of_rxd;
	 *   USART_InitStructure.USART_BaudRate = config.bandrate;
	 *   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	 *   USART_InitStructure.USART_StopBits = USART_StopBits_1;
	 *   USART_InitStructure.USART_Parity = USART_Parity_No;
	 *   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 *   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	 *   USART_RECV_CALLBACK recvcallback;
	 * };
	 */
	struct USARTto485Define conf[] = {
		{ RS485_NO_1, USART1,		   1,		     GPIOD,		GPIO_Pin_12,		       GPIOA, GPIO_Pin_10, GPIOA, GPIO_Pin_9,
		  { 9600,	USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx,
		  USART_HardwareFlowControl_None }, 0 },
		{ RS485_NO_2, USART3,		   1,		     GPIOD,		GPIO_Pin_10,		       GPIOD, GPIO_Pin_9,  GPIOD, GPIO_Pin_8,
		  { 9600,	USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx,
		  USART_HardwareFlowControl_None }, 0 },
	};
	USART_To485_Configuration( conf, ARRAY_SIZE( conf, struct USARTto485Define ) );
}


void init_uart()
{
	struct USARTDefine conf[] = {
		{ GPIOC, GPIO_Pin_12, GPIOD, GPIO_Pin_2, 115200, 0 }
	};
	USART_Configuration( conf, ARRAY_SIZE( conf, struct USARTDefine ) );
}


void init_led()
{
	/*
	 * struct LedDefine{
	 *   enum LED_NO led_no;
	 *   uint16_t pin;
	 *   GPIO_TypeDef* pin_group;
	 * };
	 */
	struct LedDefine conf[] = {
		{ LED_NO_1, GPIOA, GPIO_Pin_11, LEDOFF },
		{ LED_NO_2, GPIOA, GPIO_Pin_12, LEDOFF },
		{ LED_NO_3, GPIOA, GPIO_Pin_15, LEDOFF },
		{ LED_NO_4, GPIOB, GPIO_Pin_0,	LEDOFF },
		{ LED_NO_5, GPIOB, GPIO_Pin_8,	LEDOFF },
		{ LED_NO_6, GPIOC, GPIO_Pin_8,	LEDOFF }, // LCD Backlight
		{ LED_NO_7, GPIOC, GPIO_Pin_13, LEDOFF }
	};
	LED_Configuration( conf, ARRAY_SIZE( conf, struct LedDefine ) );
}


void init_sht10()
{
	/*
	 * struct SHT10Define{
	 *   enum SHT10_NO sht10_no;
	 *   GPIO_TypeDef* pin_of_data_group;
	 *   uint16_t pin_of_data;
	 *   GPIO_TypeDef* pin_of_sck_group;
	 *   uint16_t pin_of_sck;
	 * };
	 */
	struct SHT10Define conf[] = {
		{ SHT10_NO_1, GPIOD, GPIO_Pin_14, GPIOD, GPIO_Pin_15 }
	};
	SHT10_Configuration( conf, ARRAY_SIZE( conf, struct SHT10Define ) );
	SHT10_ConReset( SHT10_NO_1 );
}


void init_adc()
{
	/*
	 * struct ADCDefine{
	 *   enum ADC_NO adc_no;
	 *   ADC_TypeDef * adc_type;
	 *   GPIO_TypeDef* pin_group;
	 *   uint16_t pin;
	 * };
	 */
	struct ADCDefine conf[] = { { ADC_NO_1, ADC1, GPIOA, GPIO_Pin_0 } };
	ADC_Configuration( conf, ARRAY_SIZE( conf, struct ADCDefine ) );
}


void init_relay()
{
	/*
	 * struct RelayDefine{
	 *   enum RELAY_NO relay_no;
	 *   GPIO_TypeDef* pin_group;
	 *   uint16_t pin;
	 *   enum RELAY_TYPE defaultstatus;
	 * };
	 */
    #ifndef VERSION_201
        struct RelayDefine conf[] = {
            { RELAY_NO_1, GPIOE, GPIO_Pin_12, RELAY_OFF },
            { RELAY_NO_2, GPIOE, GPIO_Pin_13, RELAY_OFF },
            { RELAY_NO_3, GPIOE, GPIO_Pin_14, RELAY_OFF },
            { RELAY_NO_4, GPIOE, GPIO_Pin_15, RELAY_OFF }
        };
    #else
        struct RelayDefine conf[] = {
            { RELAY_NO_1, GPIOB, GPIO_Pin_1, RELAY_OFF },
            { RELAY_NO_2, GPIOC, GPIO_Pin_3, RELAY_OFF },
            { RELAY_NO_3, GPIOC, GPIO_Pin_2, RELAY_OFF },
            { RELAY_NO_4, GPIOC, GPIO_Pin_0, RELAY_OFF }
        };
    #endif
	Relay_Configuration( conf, ARRAY_SIZE( conf, struct RelayDefine ) );
}


void init_button()
{
	/*
	 * struct ButtonDefine{
	 *   enum BUTTON_NO btn_no;
	 *   GPIO_TypeDef* pin_group;
	 *   uint16_t pin;
	 *   uint8_t gpio_portsource;
	 *   uint8_t gpio_pinsource;
	 *   uint8_t exit_line_no;
	 *   enum IRQn irqn;
	 *   ButtonCallback callback; // Not Used
	 * };
	 */
	struct ButtonDefine conf[] = {
    #ifndef VERSION_201        
		BTNDEF_0_4( 1, B, 1 ),
		BTNDEF_0_4( 2, C, 2 ),
		BTNDEF_0_4( 3, C, 3 ),
		BTNDEF_0_4( 4, C, 0 )
    #else
        BTNDEF_10_15( 1, E, 12 ),
		BTNDEF_10_15( 2, E, 13 ),
		BTNDEF_10_15( 3, E, 14 ),
		BTNDEF_10_15( 4, E, 15 )
    #endif
	};
	Button_Configuration( conf, ARRAY_SIZE( conf, struct ButtonDefine ) );
}


void init_di()
{
	/*
	 * struct DiDefine{
	 *   enum DI_NO di_no;
	 *   GPIO_TypeDef* pin_group;
	 *   uint16_t pin;
	 *   uint8_t gpio_portsource;
	 *   uint8_t gpio_pinsource;
	 *   uint32_t exit_line_no;
	 *   IRQn_Type irqn;
	 * };
	 */
	struct DiDefine conf[] = {
		DIDEF_0_4( 1, E, 0 ),
		DIDEF_0_4( 2, E, 1 ),
		DIDEF_0_4( 3, E, 2 ),
		DIDEF_0_4( 4, E, 3 ),
		DIDEF_0_4( 5, E, 4 ),
		DIDEF_5_9( 6, E, 5 ),
		DIDEF_5_9( 7, E, 6 ),
		DIDEF_5_9( 8, E, 7 )
	};
	Di_Configuration( conf, ARRAY_SIZE( conf, struct DiDefine ) );
}


void init_eth()
{
	stm32f107_eth_init();
	Eth_LwipRegisterWithDHCP();
//    Eth_LwipRegisterWithGivenIP("192.168.1.3","255.255.0.0","192.168.1.1");
}
