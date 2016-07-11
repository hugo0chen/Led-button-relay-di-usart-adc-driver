#include "stm32f10x_adc.h"
#include "ADC.h"
#include "Delay.h"
#include <string.h>

struct ADCDefine _adc_defines[ADC_MAX_SIZE];
#define ADC_GetDef(no) (_adc_defines[no])

//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3
void ADC_Configuration(const struct ADCDefine* defines, uint8_t len)
{
    uint8_t a;
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
	memset( (void*)&_adc_defines[0], 0, sizeof(_adc_defines) );

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

    for (a=0;a<len;++a)    {
        struct ADCDefine def = defines[a];
        _adc_defines[def.adc_no] = def;

        // ��Ϊģ��ͨ����������
        GPIO_InitStructure.GPIO_Pin = def.pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(def.pin_group, &GPIO_InitStructure);

        ADC_DeInit(def.adc_type);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
        ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
        ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
        ADC_Init(def.adc_type, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

        ADC_Cmd(def.adc_type, ENABLE);	//ʹ��ָ����ADC1
        ADC_ResetCalibration(def.adc_type);	//ʹ�ܸ�λУ׼
        while (ADC_GetResetCalibrationStatus(def.adc_type));	//�ȴ���λУ׼����
        ADC_StartCalibration(def.adc_type);	 //����ADУ׼
        while (ADC_GetCalibrationStatus(def.adc_type));	 //�ȴ�У׼����
        // ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
    }
}


//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 ADC_Get(enum ADC_NO adc_no, u8 ch)
{
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
    struct ADCDefine def = ADC_GetDef(adc_no);
    ADC_RegularChannelConfig(def.adc_type, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����

    ADC_SoftwareStartConvCmd(def.adc_type, ENABLE);		//ʹ��ָ����ADC1�����ת����������

    while (!ADC_GetFlagStatus(def.adc_type, ADC_FLAG_EOC ));//�ȴ�ת������

    return ADC_GetConversionValue(def.adc_type);	//�������һ��ADC1�������ת�����
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




























