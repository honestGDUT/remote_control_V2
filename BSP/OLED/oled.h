#ifndef __OLED__H
#define __OLED__H			  	 

#include "main.h"

/*--------------------引脚定义--------------------------*/

#define OLED_RES_HIGH	HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)		//PB0（RES）输出高
#define OLED_RES_LOW		HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)	//PB0（RES）输出低

#define OLED_DC_HIGH		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)		//PB1（DC）输出高
#define OLED_DC_LOW		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)	//PB1（DC）输出低

#define SPI_CS_HIGH		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)		//PA4（CS）输出高
#define SPI_CS_LOW		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)	//PA4（CS）输出低


/*definition--------------------------------------------*/
#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据

#define SIZE 		16		//显示字符的大小
#define Max_Column	128		//最大列数
#define Max_Row		64		//最大行数
#define X_WIDTH 	128		//X轴的宽度
#define Y_WIDTH 	64	    //Y轴的宽度					  				   


/*--------------------------------------函数声明----------------------------------------*/
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);	  //OLED写字节函数
void OLED_Display_On(void);	//开显示函数
void OLED_Display_Off(void);//关显示函数
void OLED_Init(void);	//OLED初始化函数
void OLED_Clear(void);  //清屏函数
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr); //显示字符函数
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2); //在指定的位置，显示一个指定数的长度大小函数
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);	 //在指定位置开始显示字符串函数
void OLED_Set_Pos(unsigned char x, unsigned char y);		//画点函数
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no); //声明在指定位置显示汉字函数
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]); //显示图片函数
void OLED_Scroll(void);	//滚动函数

#endif  
	 



