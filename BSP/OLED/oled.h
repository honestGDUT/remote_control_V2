#ifndef __OLED__H
#define __OLED__H			  	 

#include "main.h"

/*--------------------���Ŷ���--------------------------*/

#define OLED_RES_HIGH	HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)		//PB0��RES�������
#define OLED_RES_LOW		HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)	//PB0��RES�������

#define OLED_DC_HIGH		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)		//PB1��DC�������
#define OLED_DC_LOW		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)	//PB1��DC�������

#define SPI_CS_HIGH		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)		//PA4��CS�������
#define SPI_CS_LOW		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)	//PA4��CS�������


/*definition--------------------------------------------*/
#define OLED_CMD  	0		//д����
#define OLED_DATA 	1		//д����

#define SIZE 		16		//��ʾ�ַ��Ĵ�С
#define Max_Column	128		//�������
#define Max_Row		64		//�������
#define X_WIDTH 	128		//X��Ŀ��
#define Y_WIDTH 	64	    //Y��Ŀ��					  				   


/*--------------------------------------��������----------------------------------------*/
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);	  //OLEDд�ֽں���
void OLED_Display_On(void);	//����ʾ����
void OLED_Display_Off(void);//����ʾ����
void OLED_Init(void);	//OLED��ʼ������
void OLED_Clear(void);  //��������
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr); //��ʾ�ַ�����
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2); //��ָ����λ�ã���ʾһ��ָ�����ĳ��ȴ�С����
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);	 //��ָ��λ�ÿ�ʼ��ʾ�ַ�������
void OLED_Set_Pos(unsigned char x, unsigned char y);		//���㺯��
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no); //������ָ��λ����ʾ���ֺ���
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]); //��ʾͼƬ����
void OLED_Scroll(void);	//��������

#endif  
	 



