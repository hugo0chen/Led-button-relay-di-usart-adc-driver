#include "relay.h"
#include <string.h>

struct RelayDefine _relay_defines[RELAY_MAX_SIZE];

uint8_t Relay_Configuration(struct RelayDefine conf[], int len)
{
    uint8_t l;
    GPIO_InitTypeDef	GPIO_InitStructure;
	
	memset( (void*)&_relay_defines[0], 0, sizeof(_relay_defines) );

    for (l=0;l<len;++l)
    {
        const struct RelayDefine def = conf[l];
        _relay_defines[def.relay_no] = def;

        GPIO_InitStructure.GPIO_Pin = def.pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(def.pin_group, &GPIO_InitStructure);
        if (def.defaultstatus == RELAY_ON)
        {
            Relay_ON(def.relay_no);
        }
        else
        {
            Relay_OFF(def.relay_no);
        }
    }
    return 0;
}

uint8_t Relay_ON(enum RELAY_NO relay_no)
{
    if (_relay_defines[relay_no].pin!=0)
    {
        GPIO_ResetBits(_relay_defines[relay_no].pin_group, _relay_defines[relay_no].pin);
				//GPIO_SetBits(_relay_defines[relay_no].pin_group, _relay_defines[relay_no].pin);
        return 0;
    }
    return 1;
}

uint8_t Relay_OFF(enum RELAY_NO relay_no)
{
    if (_relay_defines[relay_no].pin!=0)
    {
        GPIO_SetBits(_relay_defines[relay_no].pin_group, _relay_defines[relay_no].pin);
				//GPIO_ResetBits(_relay_defines[relay_no].pin_group, _relay_defines[relay_no].pin);
        return 0;
    }
    return 1;
}


