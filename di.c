#include "di.h"
#include <string.h>

struct DiDefine _di_defines[DI_MAX_SIZE];

uint8_t Di_Configuration(struct DiDefine conf[], int len){
    uint8_t b;
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

  	memset( (void*)&_di_defines[0], 0, sizeof(_di_defines) );

    for (b=0;b<len;++b)    {
        struct DiDefine def = conf[b];
        _di_defines[def.di_no] = def;

        GPIO_InitStructure.GPIO_Pin = def.pin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(def.pin_group, &GPIO_InitStructure);
        
        
        EXTI_InitStructure.EXTI_Line    = def.exit_line_no;
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);
        GPIO_EXTILineConfig(def.gpio_portsource, def.gpio_pinsource);

        NVIC_InitStructure.NVIC_IRQChannel = def.irqn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    return 0;
}

bool Di_IsIN(enum DI_NO di_no)
{
    struct DiDefine def = _di_defines[di_no];
    return (bool)(GPIO_ReadInputDataBit(def.pin_group, def.pin) == Bit_RESET);
}
