#ifndef __U8G2_INIT_
#define __U8G2_INIT_


#include "main.h"
#include "u8g2.h"
void delay_us(uint32_t udelay);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);


#endif
