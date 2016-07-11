#ifndef _ETH_H_
#define _ETH_H_

/*
 * @brief 使用DHCP启动网卡
 */
int Eth_LwipRegisterWithDHCP(void);

/*
 * @brief 使用固定IP启动网卡
 * @param ip IP地址，字符串(x.x.x.x)
 * @param mark 子网掩码，字符串(x.x.x.x)
 * @param gateway 网关，字符串(x.x.x.x)
 */
int Eth_LwipRegisterWithGivenIP(const char* ip, const char* netmask, const char* gateway);
/*
 * @brief 初始化STM32F107网卡
 */
void stm32f107_eth_init(void);

/*
 * @brief 获取IP地址，字符串(x.x.x.x)
 */
const char* Eth_LwipGetIP(void);

/*
 * @brief 获取MAC地址(二进制)
 */
const unsigned char* Eth_GetMAC(void);

/*
 @brief 是否在线
 @ret 0=false, 1=true
 */
int Eth_IsOnline(void);

#endif
