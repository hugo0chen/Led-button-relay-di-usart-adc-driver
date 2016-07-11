#ifndef _ETH_H_
#define _ETH_H_

/*
 * @brief ʹ��DHCP��������
 */
int Eth_LwipRegisterWithDHCP(void);

/*
 * @brief ʹ�ù̶�IP��������
 * @param ip IP��ַ���ַ���(x.x.x.x)
 * @param mark �������룬�ַ���(x.x.x.x)
 * @param gateway ���أ��ַ���(x.x.x.x)
 */
int Eth_LwipRegisterWithGivenIP(const char* ip, const char* netmask, const char* gateway);
/*
 * @brief ��ʼ��STM32F107����
 */
void stm32f107_eth_init(void);

/*
 * @brief ��ȡIP��ַ���ַ���(x.x.x.x)
 */
const char* Eth_LwipGetIP(void);

/*
 * @brief ��ȡMAC��ַ(������)
 */
const unsigned char* Eth_GetMAC(void);

/*
 @brief �Ƿ�����
 @ret 0=false, 1=true
 */
int Eth_IsOnline(void);

#endif
