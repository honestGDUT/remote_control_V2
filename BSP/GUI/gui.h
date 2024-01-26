#pragma once

#include "u8g2.h"

typedef struct Menu
{
  struct Menu* parent;
  
  struct Menu* last;
  struct Menu* next;
  
  uint8_t ID;
  uint8_t index;  
  int data; 
  void (*Function)();
 
  struct Menu* child;

}MENU;

void dispaly(float rc_value[]);
void dispaly_bmp(void);

void NoFun(void);
void Menu_Init(void);
MENU* Creat_Menu(uint8_t ID,   uint8_t index,int data, void* Function);
MENU* Creat_BrotherMenu(uint8_t ID, uint8_t index, int data, void* Function);
MENU* Creat_ChildMenu(uint8_t ID, uint8_t index, int data, void* Function);
MENU* Circle_Menu(void);
void Choose_Menu(uint8_t key_data[]);










