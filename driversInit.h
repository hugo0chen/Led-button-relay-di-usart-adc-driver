#ifndef __DRIVERS_INIT_H
#define __DRIVERS_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif 

void RCC_Configuration(void);
void init_gpio_remap(void);
void init_button(void);
void init_rs485(void);
void init_led(void);
void init_relay(void);
void init_di(void);
void init_uart(void);
void SPI1_Init(void);
void init_sht10(void);
void init_adc(void);
void init_eth(void);

#endif 

