#include "stm32f10x_adc.h"
#include "ADC.h"
#include "Delay.h"
#include <string.h>

struct ADCDefine _adc_defines[ADC_MAX_SIZE];
#define ADC_GetDef(no) (_adc_defines[no])

//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3
void ADC_Configuration(const struct ADCDefine* defines, uint8_t len)
{
    uint8_t a;
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
	memset( (void*)&_adc_defines[0], 0, sizeof(_adc_defines) );

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    for (a=0;a<len;++a)    {
        struct ADCDefine def = defines[a];
        _adc_defines[def.adc_no] = def;

        // 作为模拟通道输入引脚
        GPIO_InitStructure.GPIO_Pin = def.pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(def.pin_group, &GPIO_InitStructure);

        ADC_DeInit(def.adc_type);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
        ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
        ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
        ADC_Init(def.adc_type, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

        ADC_Cmd(def.adc_type, ENABLE);	//使能指定的ADC1
        ADC_ResetCalibration(def.adc_type);	//使能复位校准
        while (ADC_GetResetCalibrationStatus(def.adc_type));	//等待复位校准结束
        ADC_StartCalibration(def.adc_type);	 //开启AD校准
        while (ADC_GetCalibrationStatus(def.adc_type));	 //等待校准结束
        // ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
    }
}


//获得ADC值
//ch:通道值 0~3
u16 ADC_Get(enum ADC_NO adc_no, u8 ch)
{
    //设置指定ADC的规则组通道，一个序列，采样时间
    struct ADCDefine def = ADC_GetDef(adc_no);
    ADC_RegularChannelConfig(def.adc_type, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期

    ADC_SoftwareStartConvCmd(def.adc_type, ENABLE);		//使能指定的ADC1的软件转换启动功能

    while (!ADC_GetFlagStatus(def.adc_type, ADC_FLAG_EOC ));//等待转换结束

    return ADC_GetConversionValue(def.adc_type);	//返回最近一次ADC1规则组的转换结果
}

float ADC_GetAverage(enum ADC_NO adc_no, u8 ch,u8 times)
{
    u32 temp_val=0;
    u8 t;
    for (t=0;t<times;t++)
    {
        temp_val+=ADC_Get(adc_no, ch);
        Delay_nms(5);
    }
    return ((float)(temp_val/times)/4096.0)*3300;
}




























