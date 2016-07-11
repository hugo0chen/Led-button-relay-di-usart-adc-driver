#include "GlobalVar.h"
#include "RS485.h"


unsigned char receiveBuf[8];      
unsigned char receiveCount; 

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */     

/*
void Rcc_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO,ENABLE);
}


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	//定义数据结构体
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);//将中断矢量放到Flash的0地址

  NVIC_PriorityGroupConfig(BOARD_NVIC_PRIORITYGROUP);//设置优先级配置的模式，详情请阅读原材料中的文章

  //使能串口中断，并设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);	//将结构体丢到配置函数，即写入到对应寄存器中

  //使能串口中断，并设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);	//将结构体丢到配置函数，即写入到对应寄存器中
	 
}

void vRS485_1Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE); 
	
	ClrRs485EnPort1(); 
}

void vRS485_2Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3  , ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3, ENABLE); 
	
	ClrRs485EnPort2(); 
}

void UART1_SendChar(unsigned char sChar)
{
	// Write a character to the USART 
	USART_SendData(USART1, (u8) sChar);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
}

void UART1_SendString(u8* sString , u8 strLen)
{
	u8 i=0;
	SetRs485EnPort1();
	for(i=0;i<strLen;i++)
    {
        UART1_SendChar(*sString++);
    }
	ClrRs485EnPort1();
}


void USART1_IRQHandler(void)
{
	unsigned char receiveChar;         //Char by UART received
		
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		
		receiveChar =USART_ReceiveData(USART1);		
		UART1RxBuffer[0] = receiveChar;
	}
}
*/

/*
void UART3_SendChar(unsigned char sChar)
{
	// Write a character to the USART 
	USART_SendData(USART3, (u8) sChar);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{
	}
}


void UART3_SendString(u8* sString , u8 strLen)
{
	u8 i=0;
	SetRs485EnPort2();
	for(i=0;i<strLen;i++)
    {
        UART3_SendChar(*sString++);
    }
	ClrRs485EnPort2();
}*/

/*
void USART3_IRQHandler(void)
{
	unsigned char receiveChar;         //Char by UART received
		
	if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		
		receiveChar =USART_ReceiveData(USART3);		
		UART1RxBuffer[0] = receiveChar;
	}
}*/

//不使用半主机模式
#if 1 //如果没有这段，则需要在target选项中选择使用USE microLIB
#pragma import(__use_no_semihosting)
struct __FILE  
{  
	int handle;  
};  
FILE __stdout;  

_sys_exit(int x)  
{  
	x = x;  
}
#endif





/************************** new *****************************/

USART_RECV_CALLBACK _usart_recv_callbacks[4] = {0};
uint16_t _usart_485_trx_pins[4] = {0};
USART_TypeDef* _usart_type_defines[4] = {0, USART1, USART2, USART3};

void _usart1_config_to_rs485(struct USART_TO_485 config)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);				 
	GPIO_InitStructure.GPIO_Pin =  config.pin_of_tre ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = config.pin_of_txd;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = config.pin_of_rxd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = config.bandrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE); 
	
	GPIO_ResetBits(GPIOD, config.pin_of_tre);
}

void _usart2_config_to_rs485(struct USART_TO_485 config)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_PinRemapConfig(GPIO_Remap_USART2  , ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);				 
	GPIO_InitStructure.GPIO_Pin =  config.pin_of_tre ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = config.pin_of_txd;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = config.pin_of_rxd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = config.bandrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3, ENABLE); 
	
	GPIO_ResetBits(GPIOD, config.pin_of_tre);
}

void _usart3_config_to_rs485(struct USART_TO_485 config)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3  , ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);				 
	GPIO_InitStructure.GPIO_Pin =  config.pin_of_tre ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = config.pin_of_txd;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = config.pin_of_rxd;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = config.bandrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3, ENABLE); 
	
	GPIO_ResetBits(GPIOD, config.pin_of_tre);
}


void _usart_nvic_config(uint32_t usart_irqn, uint32_t preemptionpriority, uint32_t subpriority)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = usart_irqn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionpriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
	 
}


void _usart_comm_irqhandler(USART_TypeDef* usarttypedef, enum BOARD_USART_TYPE usart_type){
	if(_usart_recv_callbacks[usart_type]!=0){
		unsigned char recv_char;
		if(USART_GetFlagStatus(usarttypedef,USART_FLAG_RXNE) != RESET)
		{
			USART_ClearITPendingBit(usarttypedef, USART_IT_RXNE);		
			recv_char =USART_ReceiveData(usarttypedef);		
			_usart_recv_callbacks[usart_type](usart_type, recv_char);
		}
	}
}

void USART1_IRQHandler(void)
{
	_usart_comm_irqhandler(USART1, BOARD_USART_1);
}

void USART2_IRQHandler(void)
{
	_usart_comm_irqhandler(USART2, BOARD_USART_2);
}

void USART3_IRQHandler(void)
{
	_usart_comm_irqhandler(USART3, BOARD_USART_3);
}

#define SetRs485EnPort(usart_no)  GPIO_SetBits(GPIOD, _usart_485_trx_pins[usart_no])    
#define ClrRs485EnPort(usart_no)  GPIO_ResetBits(GPIOD, _usart_485_trx_pins[usart_no])  
#define UartIsConfigurated(usart_no) (_usart_485_trx_pins[usart_no]!=0)
#define UartTypeDef(usart_no) (_usart_type_defines[usart_no])

void _usart_sendchar(USART_TypeDef* usart_typedef, uint8_t sChar)
{
	// Write a character to the USART 
	USART_SendData(usart_typedef, (uint8_t) sChar);
	while(USART_GetFlagStatus(usart_typedef, USART_FLAG_TXE) == RESET)
	{
	}
}

uint32_t USART_SendString(enum BOARD_USART_TYPE usart_type, uint8_t* str , uint8_t strlen)
{
	uint8_t i;
	if(UartIsConfigurated(usart_type)){
		SetRs485EnPort(usart_type);
		for(i=0;i<strlen;i++)
		{
				_usart_sendchar( UartTypeDef(usart_type), *(str++));
		}
		ClrRs485EnPort(usart_type);
	}	
}

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(u8)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}

void _usart_rcc_config(uint8_t apb1_enable, uint8_t apb2_enable){
	if(apb2_enable>0){
		/*
		只有USART1在APB2上
		*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO,ENABLE);
	}
	if(apb1_enable>0){
		// TODO apb1_enable
	}
}


uint32_t Init_USART_to_485(struct USART_TO_485* configs, uint16_t len){
	uint16_t u=0;
	uint8_t apb1_enable=0;
	uint8_t apb2_enable=0;
	for(u=0; u<len; ++u){
		struct USART_TO_485 config = configs[u];
		switch(config.usart_no){
			case BOARD_USART_1:{
				_usart1_config_to_rs485(config);
				_usart_nvic_config(USART1_IRQn, config.preemption_priority, 0);
				++apb2_enable; // USART1 在APB2上
				break;
			}
			case BOARD_USART_2:{
				_usart2_config_to_rs485(config);
				_usart_nvic_config(USART2_IRQn, config.preemption_priority, 0);
				++apb1_enable; // USART2 在APB1上
				break;
			}
			case BOARD_USART_3:{
				_usart3_config_to_rs485(config);
				_usart_nvic_config(USART3_IRQn, config.preemption_priority, 0);
				++apb1_enable; // USART3 在APB1上
				break;
			}
			default:{
				return 1;
			}
		}
		_usart_recv_callbacks[config.usart_no]=config.recvcallback;
		_usart_485_trx_pins[config.usart_no]=config.pin_of_tre;
		return 0;
	}	
}







