#include "u8g2_init.h"
#include "main.h"
#include "u8g2.h"
#include "spi.h"
#include "oled.h"

void delay_us(uint32_t udelay)
{
  uint32_t startval,tickn,delays,wait;
 
  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
  delays =udelay * 72; //sysc / 1000 * udelay;
  if(delays > startval)
    {
      while(HAL_GetTick() == tickn)
        {
 
        }
      wait = 72000 + startval - delays;
      while(wait < SysTick->VAL)
        {
 
        }
    }
  else
    {
      wait = startval - delays;
      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
 
        }
    }
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg){		
    case U8X8_MSG_BYTE_SEND:       
      HAL_SPI_Transmit(&hspi1, (uint8_t*)arg_ptr, arg_int, 200);
    break;			
    case U8X8_MSG_BYTE_INIT:
      /* add your custom code to init i2c subsystem */
    break;	
    case U8X8_MSG_BYTE_SET_DC:
      HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, arg_int);
    break;
		case U8X8_MSG_BYTE_START_TRANSFER:
      SPI_CS_LOW;
    break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      SPI_CS_HIGH;
    break;
		
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        HAL_Delay(1);
        break;
    default:
        break;
    }
    return 1;
}





void u8g2Init(u8g2_t *u8g2)
{
  u8g2_Setup_sh1106_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_gpio_and_delay);  // init the u8g2 of struct
//	u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_gpio_and_delay);  // init the u8g2 of struct
	u8g2_InitDisplay(u8g2); // Initialization work is carried out according to the selected chip, and after initialization, the display is turned off
	u8g2_SetPowerSave(u8g2, 0); // Turn on the monitor
	u8g2_ClearBuffer(u8g2);
}




