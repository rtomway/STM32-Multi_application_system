#ifndef MYU8G2_H_
#define MYU8G2_H_

#include "u8g2/u8g2.h"

void myU8g2_Init(u8g2_t *u8g2);

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

extern u8g2_t u8g2;

#endif /* MYU8G2_H_ */
