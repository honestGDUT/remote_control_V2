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

//��ʱ��ɨ�谴���жϺ�����ÿ10MSɨ��һ�ΰ���ֵ
//֧�ֵ�����˫��������
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
                      key[i].judge_sta=2;   //ȥ��                        
                  }
                  else
                  {
                      key[i].judge_sta = 0;
                  }
                  break;
              case 2:         //��������
                  if((key[i].key_sta ==1)&&key[i].key_time<70)
                  {
                      if(key[i].double_click_timeEN == 0)
                      {
                          key[i].double_click_timeEN = 1;
                          key[i].double_click_time = 0;
                      }
                      else      //�ڼ�ʱ��Χ���ְ���һ�� 
                      {
                          key[i].key_event = double_flag;   //˫�����
                          key[i].double_click_timeEN = 0;
                      }
                      key[i].judge_sta = 0;                                              
                  }  
                  else if(key[i].key_sta == 1&&key[i].key_time>=70)  //�ɿ����ǳ�����
                  {

                      key[i].judge_sta = 0;
                  }
                  else
                  {
                      if(key[i].key_time>70)   //����
                          key[i].key_event = long_flag; 
                       key[i].key_time++; //��������ʱ ��û�ɿ�
                  }
                  break;                
              default:break;                  
          }
          if(key[i].double_click_timeEN == 1)   //��ʱȷ���Ƿ�˫��
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
      
      //�����ֵû�б仯����д�뻺�����
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

