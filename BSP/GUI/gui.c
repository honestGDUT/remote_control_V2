#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "gui.h"
#include "key.h"
#include "oled.h"
#include "u8g2.h"
#include "u8g2_init.h"
#include "queue.h"
#include "communication.h"


#define First_x 32
#define First_y 40
#define Second_x 100
#define Second_y 40
#define Rad      18
#define Line_Len 10

MENU* nowMenu;
MENU* LRMenu;
u8g2_t u8g2;

#define KEY_BUF_SIZE 128
extern CircularQueue KEY_Queue_Hand;
extern uint8_t Key_Buf[KEY_BUF_SIZE];

char show[][20] = {
  "MENU", "Settings", "Data", "Download","Save",
  "Child_01","Child_02","Child_03",
  "Child_011","Child_0112","Child_013"
};



void Menu_Init(void)
{
    u8g2Init(&u8g2);
  
    nowMenu = Creat_Menu(0, 1, 0, *NoFun);
        nowMenu = Creat_ChildMenu(1, 1, 0, *NoFun);
            nowMenu = Creat_ChildMenu(5, 1, 0, *NoFun);
              nowMenu = Creat_ChildMenu(8, 1, 0, *NoFun);
              nowMenu = Creat_BrotherMenu(9,  2, 0, *NoFun);
              nowMenu = Creat_BrotherMenu(10, 3, 0, *NoFun);
              nowMenu = Circle_Menu();
            nowMenu = Creat_BrotherMenu(6, 2, 0, *NoFun);
            nowMenu = Creat_BrotherMenu(7, 3, 0, *NoFun);
            nowMenu = Circle_Menu();
        nowMenu = Creat_BrotherMenu(2, 2, 0, *NoFun);
        nowMenu = Creat_BrotherMenu(3, 3, 0, *NoFun);
        nowMenu = Creat_BrotherMenu(4, 4, 0, *NoFun);
        nowMenu = Circle_Menu();
    nowMenu = Circle_Menu();
  
    nowMenu = nowMenu->child;
    LRMenu = nowMenu; 
}
  
MENU* Creat_Menu(uint8_t ID, uint8_t index, int data, void* Function)
{
  MENU* node = malloc(sizeof(MENU));
  node->ID = ID;
  node->index = index;
  node->data = data;
  node->Function = Function;
  
  node->last = NULL;
  node->next = NULL;
  node->child = node;  
  node->parent = node;

  return node;
}

MENU* Creat_ChildMenu(uint8_t ID,uint8_t index, int data, void* Function)
{
  MENU* node = malloc(sizeof(MENU));
  node->ID = ID;
  node->index = index;
  node->data = data;
  node->Function = Function;
  
  node->last = NULL;
  node->next = NULL;
  node->child = node;
  node->parent = nowMenu;
  
  nowMenu->child = node;
  
  return node;
}

MENU* Creat_BrotherMenu(uint8_t ID,uint8_t index, int data, void* Function)
{
  MENU* node = malloc(sizeof(MENU));
  node->ID = ID;
  node->index = index;
  node->data = data;
  node->Function = Function;
  
  node->last = nowMenu;
  node->next = NULL;
  node->parent = nowMenu->parent;
  node->child = node;
  
  nowMenu->next = node;
  
  return node;
  
}



MENU* Circle_Menu(void)
{
  MENU* finalMenu = nowMenu;
    
  while(nowMenu->last != NULL)
  {
    nowMenu = nowMenu->last;
    
  }
  nowMenu->last = finalMenu;
  finalMenu->next = nowMenu;
  
  return nowMenu->parent;
}


void Choose_Menu(uint8_t key_data[])
{  
  if(key_data[0] == 1)
  {
    LRMenu = LRMenu->last;
  }
  if(key_data[1] == 1)
  { 
    if(LRMenu->index <3)
    {
      LRMenu = LRMenu->next;
    }
    else if(LRMenu->index >=3 && LRMenu->next->index !=1)
    {
       LRMenu = LRMenu->next;
      nowMenu = nowMenu->next;
    }
    
  }  
  
  if(key_data[2] == 1)
  {
    if(nowMenu->parent->ID != 0)
      nowMenu = nowMenu->parent;
  }  
   if(key_data[3] == 1)
  {
//    nowMenu = LRMenu;
    nowMenu = nowMenu->child;
  } 
  
  MENU* now = nowMenu; 
   
  u8g2_ClearBuffer(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  
  char str_buf[10];
  
  uint8_t order;

  order = LRMenu->index;
  if(order>3)
    order = 3;
  sprintf(str_buf,"%d",0);
  u8g2_DrawStr(&u8g2, 120, 30+(order-1)*15,str_buf);
  
  u8g2_DrawStr(&u8g2,64-strlen(show[nowMenu->parent->ID])*5/2+1,13,show[nowMenu->parent->ID]);
  
  
  sprintf(str_buf,"%d.", nowMenu->ID);
  u8g2_DrawStr(&u8g2, 1, 30,str_buf);
  u8g2_DrawStr(&u8g2,15,30,show[nowMenu->ID]);
  
  nowMenu = now->next;  
  sprintf(str_buf,"%d.", nowMenu->ID);
  u8g2_DrawStr(&u8g2, 1, 45,str_buf);
  u8g2_DrawStr(&u8g2,15,45,show[nowMenu->ID]);

  nowMenu = now->next->next;
  sprintf(str_buf,"%d.", nowMenu->ID);
  u8g2_DrawStr(&u8g2, 1, 60,str_buf);
  u8g2_DrawStr(&u8g2,15,60,show[nowMenu->ID]);


  u8g2_SendBuffer(&u8g2);
   
  nowMenu = now;
}

void NoFun(void)
{

}

const unsigned char gImage_signal[] = { /* 0X01,0X01,0X40,0X00,0X40,0X00, */
0x00,0x00,0x00,0x00,0xFE,0x0F,0x04,0x00,0x46,0x0C,0x05,0x00,0x48,0x42,0x05,0x00,
0xF0,0x51,0x05,0x00,0x40,0x54,0x05,0x00,0x40,0x54,0x05,0x00,0x40,0x54,0x05,0x00
};

const unsigned char gImage_unsignal[] = {
0x00,0x00,0x00,0x00,0xFE,0x0F,0x00,0x00,0x46,0x2C,0x04,0x00,0x48,0x42,0x02,0x00,
0xF0,0x81,0x01,0x00,0x40,0x80,0x01,0x00,0x40,0x40,0x02,0x00,0x40,0x20,0x04,0x00,
};

const unsigned char gImage_poshold[] = {
0x10,0x00,0x02,0x20,0x00,0x7E,0xFF,0x07,0x02,0x01,0x04,0x02,0x00,0xE0,0x3F,0xFF,
0x27,0x20,0x20,0x20,0x20,0x24,0x20,0x20,0xE4,0xE3,0x3F,0x24,0x00,0x00,0x2A,0xA0,
0x24,0xF1,0x17,0x49,/*"D:\下载\poshold.bmp",0*/
};

const unsigned char gImage_althold[] = {
0x10,0x00,0x02,0x20,0xF0,0x7F,0xFF,0x07,0x00,0x01,0xC4,0x1F,0x00,0x40,0x10,0xFF,
0xC7,0x1F,0x20,0x00,0x00,0x24,0xF0,0x7F,0xE4,0x13,0x40,0x24,0x90,0x4F,0x2A,0x90,
0x48,0xF1,0x97,0x6F,/*"D:\下载\text-image (1).bmp",0*/
}; 

void dispaly_bmp(void)
{
  u8g2_ClearBuffer(&u8g2); 

      
	u8g2_SendBuffer(&u8g2); 
}

extern float user_data[4];

void dispaly(float rc_value[])
{ 
    int8_t offset_x1 = (rc_value[1]-50)/2.5;
    int8_t offset_y1 = (rc_value[0]-50)/2.5; 
    int8_t offset_x2 = (rc_value[2]-50)/2.5;
    int8_t offset_y2 = (rc_value[3]-50)/2.5;
    u8g2_ClearBuffer(&u8g2); 
    
    /*设置字体和字符缓存数组 */
    u8g2_SetFont(&u8g2,u8g2_font_5x7_tf );
    char strbuf[32];
    /*设置字体和字符缓存数组 */
  
    /*遥控信号图标 */
    set_rc_connected();
    if(get_rc_connected() > 10)
      u8g2_DrawXBMP(&u8g2, 2, 2, 32, 8, gImage_unsignal);   
    else
      u8g2_DrawXBMP(&u8g2, 2, 2, 32, 8, gImage_signal);   
   /*遥控信号图标 */
    
    /*飞行模式 */

     sprintf(strbuf, "Mode:");
     u8g2_DrawStr(&u8g2, 64-30-strlen(strbuf), 10, strbuf); 
    if(get_rc_connected() < 10)
    {
      if(get_mf_mode() == mf_mission)
      {
        u8g2_DrawXBMP(&u8g2, 64-23/2, 0, 23, 12, gImage_poshold);  
      }
      else if(get_mf_mode() == mf_althold)
      {
        u8g2_DrawXBMP(&u8g2, 64-23/2, 0, 23, 12, gImage_althold);   
      }
    }
    else
    {
     sprintf(strbuf, "Error");
     u8g2_DrawStr(&u8g2, 64-5-strlen(strbuf), 10, strbuf); 
    }
    /*飞行模式 */
    
    /*遥控器电池电压*/
    sprintf(strbuf, "Bat:%.1fV", rc_value[4]);
    u8g2_DrawStr(&u8g2, 64-20, 64, strbuf);
    /*遥控器电池电压*/
    
    /*电池电压*/
    float voltage = get_voltage();
    sprintf(strbuf, "Bat:%.1fV", voltage);
    u8g2_DrawStr(&u8g2, 85, 10, strbuf);     
    /*电池电压*/
    
    /*圆*/
    u8g2_DrawCircle(&u8g2,First_x,  First_y,  Rad, U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2,Second_x, Second_y, Rad,U8G2_DRAW_ALL);

    u8g2_DrawHLine(&u8g2, First_x+Rad-5/2,  First_y, 5);
    u8g2_DrawHLine(&u8g2, First_x-Rad-5/2,  First_y, 5);    
    u8g2_DrawVLine(&u8g2, First_x,          First_y+Rad-5/2, 5);
    u8g2_DrawVLine(&u8g2, First_x,          First_y-Rad-5/2, 5);

    u8g2_DrawHLine(&u8g2, Second_x+Rad-5/2,  Second_y, 5);
    u8g2_DrawHLine(&u8g2, Second_x-Rad-5/2,  Second_y, 5);    
    u8g2_DrawVLine(&u8g2, Second_x,          Second_y+Rad-5/2, 5);
    u8g2_DrawVLine(&u8g2, Second_x,          Second_y-Rad-5/2, 5);
    /*圆*/


//    sprintf(strbuf, "%.1f", rc_value[0]);
//    u8g2_DrawStr(&u8g2, First_x-10, 64, strbuf);

//    sprintf(strbuf, "%.1f", rc_value[1]);
//    u8g2_DrawStr(&u8g2, 0, 20, strbuf);
//    
//    sprintf(strbuf, "%.1f", rc_value[2]);
//    u8g2_DrawStr(&u8g2, Second_x-10, 64, strbuf);
//    
//    sprintf(strbuf, "%.1f", rc_value[3]);
//    u8g2_DrawStr(&u8g2, 75-10, 20, strbuf);    

    /*十字光标*/
    u8g2_DrawHLine(&u8g2, First_x-offset_x1-10/2, First_y-offset_y1,      Line_Len);
    u8g2_DrawVLine(&u8g2, First_x-offset_x1,      First_y-offset_y1-10/2, Line_Len);
 
 
    u8g2_DrawHLine(&u8g2, Second_x+offset_x2-10/2, Second_y+offset_y2, 10);
    u8g2_DrawVLine(&u8g2, Second_x+offset_x2,      Second_y+offset_y2-10/2, Line_Len);
    /*十字光标*/
    
		u8g2_SendBuffer(&u8g2); 
}



