#include "boardconfig.h"

uint16_t BOARD_NVIC_Init(uint16_t priority_group){
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);//���ж�ʸ���ŵ�Flash��0��ַ
  NVIC_PriorityGroupConfig(priority_group);//�������ȼ����õ�ģʽ���������Ķ�ԭ�����е�����
	return 0;
}
