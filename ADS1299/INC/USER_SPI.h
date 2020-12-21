#ifndef __USER_SPI_H__
#define __USER_SPI_H__
#include "rt/rt_api.h"

struct pi_device spi;

void SPI_INIT(void);

//---------------------------------------------------------------OBSELETE-------------------------------------------------------
#define SCLK 0
#define MISO 1
#define MOSI 2
#define CS 3

#define SPI_CS_HIGH rt_gpio_set_pin_value(0, CS, 1)
#define SPI_CS_LOW rt_gpio_set_pin_value(0, CS, 0)

#define SPI_SCLK_HIGH rt_gpio_set_pin_value(0, SCLK, 1)
#define SPI_SCLK_LOW rt_gpio_set_pin_value(0, SCLK, 0)

#define SPI_MOSI_HIGH rt_gpio_set_pin_value(0, MOSI, 1)
#define SPI_MOSI_LOW rt_gpio_set_pin_value(0, MOSI, 0)

#define SPI_MISO_READ rt_gpio_get_pin_value(0, MISO)

void SPI_GPIO_INIT();
uint8_t ADS_SPI(uint8_t data);
//---------------------------------------------------------------OBSELETE-------------------------------------------------------
#endif
