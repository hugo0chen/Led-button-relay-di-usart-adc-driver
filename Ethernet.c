#include "stm32_eth.h"
#include "stm32f10x.h"

#include "Ethernet.h"
#include "tftpserver.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "netif/etharp.h"
#include "main.h"
#include "lwip/dhcp.h"
#include "lwip/ip_addr.h"
#include "ethernetif.h"
#include "netconf.h"
#include "crc32.h"
#include "stdio.h"

#define PHY_ADDRESS 			(0x01)
#define MAX_DHCP_TRIES        4

/* UDPpcb to be binded with port 69  */
struct udp_pcb *UDPpcb;
//const unsigned char * udp_data= "hello world!\r\n";

//struct netif eth_phy_netif;
struct netif eth_phy_netif;


uint32_t _is_dhcp = 0;

bool isDHCP(){
    return (bool)( _is_dhcp != 0 );
}

void setIsDHCP(){
    _is_dhcp=1;
}


__IO uint32_t TCPTimer = 0;
__IO uint32_t ARPTimer = 0;

#ifdef LWIP_DHCP
__IO uint32_t DHCPfineTimer = 0;
__IO uint32_t DHCPcoarseTimer = 0;
#endif

__IO uint32_t DisplayTimer = 0;
uint8_t LedToggle = 4;


unsigned char _eth_phy_mac[6] = {0x8, 0x2e,0,0,0,0};

void Delay_us(u32 nus);
void LwIP_Periodic_Handle(__IO uint32_t localtime);

extern void udp_send_deal(void);
static void eth_phy_clk_init(void);
static void eth_phy_gpio_init(void);
static void eth_phy_nvic_init(void);
static void eth_phy_mco_init(void);
static void eth_phy_confugration(void);
char delay_secess(void);
int TIM_TimerIsStart(u8 tim);



void Eth_GenMAC(){
    /*
    SerialID[0] = *(unsigned int*)(0x1FFFF7E8);

    SerialID[1] = *(unsigned int*)(0x1FFFF7EC);

    SerialID[2] = *(unsigned int*)(0x1FFFF7F0);
    */
    
    unsigned char *cpu_id = (unsigned char *)0x1FFFF7E8;
    unsigned int crc32_value;
    
    crc32_value = crc32(cpu_id, 12);
    _eth_phy_mac[2] = (char)(crc32_value >> 0);
    _eth_phy_mac[3] = (char)(crc32_value >> 8);
    _eth_phy_mac[4] = (char)(crc32_value >> 16);
    _eth_phy_mac[5] = (char)(crc32_value >> 24);
};


void stm32f107_eth_init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    
    Eth_GenMAC();    
    eth_phy_clk_init();
    eth_phy_gpio_init();
    eth_phy_nvic_init();
    eth_phy_mco_init();
    eth_phy_confugration();
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
    NVIC_SetPriority (SysTick_IRQn, 1);
}


const unsigned char* Eth_GetMAC()
{
    return _eth_phy_mac;
}

static void
eth_phy_clk_init(void)
{
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx | RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);
}

static void
eth_phy_nvic_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void eth_phy_gpio_init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;// PA2=ETH_MDIO
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // ETH_MDC = PC1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; // ETH_TX_EN ETH_TXD0 ETH_TXD1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; // E1_NRST  ??PB5=PPS_OUT  £? | GPIO_Pin_14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);*/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;// ETH_REF_CLK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; // PA7=CRS_DV
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ; // PC4=RXD0,PC5=RXD1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_GetClocksFreq(&RCC_Clocks);

}

static void eth_phy_mco_init(void)
{
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);
    RCC_PLL3Config(RCC_PLL3Mul_10);
    RCC_PLL3Cmd(ENABLE);
    while ( RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET ) { }
    RCC_MCOConfig(RCC_MCO_PLL3CLK);
}

static void eth_phy_confugration(void)
{
    ETH_InitTypeDef ETH_InitStructure;

    ETH_DeInit();
    ETH_SoftwareReset();
    while ( ETH_GetSoftwareResetStatus() == SET ) { }

    ETH_StructInit(&ETH_InitStructure);

    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
}




int Eth_LwipRegisterWithDHCP(){

    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    setIsDHCP();
    mem_init();
    memp_init();

    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;

    Set_MAC_Address(_eth_phy_mac);
    netif_add(&eth_phy_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
    netif_set_default(&eth_phy_netif);

    dhcp_start(&eth_phy_netif);
    netif_set_up(&eth_phy_netif);
    return 0;
}


int Eth_LwipRegisterWithGivenIP(const char* ip, const char* netmask, const char* gateway){

    struct ip_addr _ipaddr;
    struct ip_addr _netmask;
    struct ip_addr _gateway;
    int n1,n2,n3,n4;

    mem_init();
    memp_init();
    sscanf(ip, "%d.%d.%d.%d", &n1,&n2,&n3,&n4);
    IP4_ADDR(&_ipaddr, n1, n2, n3, n4);

    sscanf(netmask, "%d.%d.%d.%d", &n1,&n2,&n3,&n4);
    IP4_ADDR(&_netmask, n1, n2, n3, n4);

    sscanf(gateway, "%d.%d.%d.%d", &n1,&n2,&n3,&n4);
    IP4_ADDR(&_gateway, n1, n2, n3, n4);

    Set_MAC_Address(_eth_phy_mac);
    netif_add(&eth_phy_netif, &_ipaddr, &_netmask, &_gateway, NULL, &ethernetif_init, &ethernet_input);
    netif_set_default(&eth_phy_netif);

    netif_set_up(&eth_phy_netif);
    return 0;
}

const char* Eth_LwipGetIP(void){
    static char buf[32] = {0};
    struct ip_addr ipaddr = eth_phy_netif.ip_addr;
    sprintf(buf, "%d.%d.%d.%d", ip4_addr1(&ipaddr), ip4_addr2(&ipaddr), ip4_addr3(&ipaddr), ip4_addr4(&ipaddr));
    return buf;
}

int Eth_IsOnline(){
    return 0 != (uint32_t)(eth_phy_netif.ip_addr.addr);
}



void LwIP_Pkt_Handle(void)
{
    /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
    ethernetif_input(&eth_phy_netif);
}

void LwIP_Periodic_Handle(__IO uint32_t localtime)
{
    /* ARP periodic process every 5s */
    if (localtime - ARPTimer >= 5000)
    {
        ARPTimer =  localtime;
        etharp_tmr();
    }

#if LWIP_DHCP
    /* Fine DHCP periodic process every 500ms */
    if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
    {
        DHCPfineTimer =  localtime;
        dhcp_fine_tmr();
    }
    /* DHCP Coarse periodic process every 60s */
    if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
    {
        DHCPcoarseTimer =  localtime;
        dhcp_coarse_tmr();
    }
#endif

}
/*UDPΩ” ’÷–∂œ∫Ø ˝*/
void ETH_IRQHandler(void)
{
    /* Handles all the received frames */
    while (ETH_GetRxPktSize() != 0)
    {
        LwIP_Pkt_Handle();
    }
    /* Clear the Eth DMA Rx IT pending bits */
    ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

}
