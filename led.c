#include "led.h"
#include <string.h>

struct LedDefine _leds_defines[LED_MAX_SIZE];

uint8_t LED_Configuration(const struct LedDefine* leds, uint8_t len)
{
    uint8_t l;
    GPIO_InitTypeDef	GPIO_InitStructure;

    memset( (void*)&_leds_defines[0], 0, sizeof(_leds_defines) );

    for (l=0;l<len;++l)
    {
        const struct LedDefine def = leds[l];
        _leds_defines[def.led_no] = def;

        GPIO_InitStructure.GPIO_Pin = def.pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(def.pin_group, &GPIO_InitStructure);
        if (def.defaultstatus == LEDON)
        {
            LED_ON(def.led_no);
        }
        else
        {
            LED_OFF(def.led_no);
        }
    }
    return 0;
}

uint8_t LED_ON(enum LED_NO led_no)
{
    if (_leds_defines[led_no].pin!=0)
    {
        GPIO_ResetBits(_leds_defines[led_no].pin_group, _leds_defines[led_no].pin);
        return 0;
    }
    return 1;
}


uint8_t LED_OFF(enum LED_NO led_no)
{
    if (_leds_defines[led_no].pin!=0)
    {
        GPIO_SetBits(_leds_defines[led_no].pin_group, _leds_defines[led_no].pin);
        return 0;
    }
    return 1;
}




