#include "boardconfig.h"

uint16_t BOARD_NVIC_Init(uint16_t priority_group){
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);//将中断矢量放到Flash的0地址
  NVIC_PriorityGroupConfig(priority_group);//设置优先级配置的模式，详情请阅读原材料中的文章
	return 0;
}
