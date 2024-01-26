#include "key.h"
#include <stdio.h>
#include "queue.h"
#include "RingBuffer.h"

static uint8_t key_raw_value[6];

keys key[KEY_NUM]={0};
RING_BUFFER_T  ringbuffer;

void key_init(void)
{
  
  ringBuffer_int(&ringbuffer);
}

//定时器扫描按键中断函数，每10MS扫描一次按键值
//支持单击、双击、长按
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance == TIM1)
 {
      key[KEY_1].key_sta      = HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);    
      key[KEY_2].key_sta     = HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);     
      key[KEY_3].key_sta   = HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
      key[KEY_4].key_sta   = HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
      key[KEY_L].key_sta  = HAL_GPIO_ReadPin(KEY_L_GPIO_Port, KEY_L_Pin);
      key[KEY_R].key_sta      = HAL_GPIO_ReadPin(KEY_R_GPIO_Port, KEY_R_Pin);
            
      for(int i=0; i<KEY_NUM; i++)
      {
          switch(key[i].judge_sta)
          {
              case 0:
                  if(key[i].key_sta==0) 
                  {
                      key[i].judge_sta=1;
                      key[i].key_time = 0;
                      key[i].h_time = 0;
                  }
                  else
                  {
                     key[i].key_event = none_flag; 
                  }
                  break;
              case 1:
                  if(key[i].key_sta==0)
                  {
                      key[i].judge_sta=2;   //去抖                        
                  }
                  else
                  {
                      key[i].judge_sta = 0;
                  }
                  break;
              case 2:         //消抖过程
                  if((key[i].key_sta ==1)&&key[i].key_time<70)
                  {
                      if(key[i].double_click_timeEN == 0)
                      {
                          key[i].double_click_timeEN = 1;
                          key[i].double_click_time = 0;
                      }
                      else      //在计时范围内又按了一次 
                      {
                          key[i].key_event = double_flag;   //双击情况
                          key[i].double_click_timeEN = 0;
                      }
                      key[i].judge_sta = 0;                                              
                  }  
                  else if(key[i].key_sta == 1&&key[i].key_time>=70)  //松开且是长按键
                  {

                      key[i].judge_sta = 0;
                  }
                  else
                  {
                      if(key[i].key_time>70)   //长按
                          key[i].key_event = long_flag; 
                       key[i].key_time++; //长按键计时 还没松开
                  }
                  break;                
              default:break;                  
          }
          if(key[i].double_click_timeEN == 1)   //延时确认是否双击
          { 
              key[i].double_click_time++;
              if(key[i].double_click_time>35)
              {
                  key[i].key_event = short_flag;
                  key[i].double_click_timeEN = 0;
              }
          }
             
          key_raw_value[i] =  key[i].key_event;
          
          
      } 
      
      //如果键值没有变化，则不写入缓冲队列
      for(int i=0; i<KEY_NUM; i++)
      {
        if(key[i].last_key_event != key[i].key_event)
        {
          ringBuffer_push_data(&ringbuffer, key_raw_value, 6);    
          break;
        }
      }
      
      for(int i=0; i<KEY_NUM; i++)
      {
        key[i].last_key_event = key[i].key_event;
      
      }
 }     

}

