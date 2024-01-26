#include "stdio.h"
#include "string.h"

#include "communication.h"
#include "usart.h"

float user_data[4];

static uint8_t rc_connected;
uint8_t r_data;

  /*状态机变量*/
		static uint8_t rc_step1 = 0;	//0：接收包头'A' 'C'
																	//1：接收1字节消息类别
																	//2：接收1字节消息长度
																	//3：接收数据包内容
																	//4：接收2字节校验
		static uint8_t rc_step2 = 0;
	
		#define MAX_SDI_PACKET_SIZE 4*4
		static uint8_t msg_type;
		static uint8_t msg_length;
		static uint8_t msg_pack[MAX_SDI_PACKET_SIZE];
		static uint8_t sumA;
		static uint8_t sumB;
		
		#define reset_SDI_RC ( rc_step1 = rc_step2 = 0 )
	/*状态机变量*/

uint8_t get_rc_connected(void)
{
  return rc_connected;
}

void set_rc_connected(void)
{
  rc_connected++;
  if(rc_connected>200)
    rc_connected=200;
}

static void reset_rc_connected(void)
{
  rc_connected=0;
}

float get_voltage(void)
{
  return user_data[0];
}

mf_mode get_mf_mode(void)
{
  if(user_data[1] == 0)
  {
    return mf_althold;
  }
  else if(user_data[1] == 50)
  {
    return mf_mission;
  }
  else if(user_data[1] == 100)
  {
    return mf_poshold ;
  }
}

void SendFloatMsg(uint8_t tx_msg_type, float* rc_values) {
    uint8_t tx_msg_pack[256];
    uint8_t tx_sumA = 0, tx_sumB = 0;
    uint16_t tx_msg_length = 32; // 8 float values * 4 bytes each

    tx_msg_pack[0] = 'A';
    tx_msg_pack[1] = 'C';
    tx_msg_pack[2] = tx_msg_type;
    tx_msg_pack[3] = tx_msg_length;

    tx_sumA += tx_msg_type;   
    tx_sumB += tx_sumA;
  
    tx_sumA += tx_msg_length;
    tx_sumB += tx_sumA;

    for (uint8_t i = 0; i < 8; ++i) 
    {
      // 针对每个 float 数据的每个字节计算校验
      uint8_t* floatBytes = (uint8_t*)&rc_values[i];
      for (uint8_t j = 0; j < sizeof(float); ++j) {
          tx_sumA += floatBytes[j];
          tx_sumB += tx_sumA;
      }
      // 这里将 float 数据写入消息
      memcpy(&tx_msg_pack[4 + i * 4], &rc_values[i], sizeof(float));
    }

    tx_msg_pack[36] = tx_sumA;
    tx_msg_pack[37] = tx_sumB;

    HAL_UART_Transmit(&huart1, tx_msg_pack, 38, HAL_MAX_DELAY);
}




void KT6368A_Init(void)
{
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&r_data, 1);
}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {     
      switch( rc_step1 )
			{
				case 0 :
					//接收包头'A''C'
					if( rc_step2 == 0 )
					{
						if( r_data == 'A' )
							rc_step2 = 1;
					}
					else
					{
						if( r_data == 'C' )
						{
							rc_step1 = 1;
							rc_step2 = 0;
							sumA = sumB = 0;
						}
						else
							rc_step2 = 0;
					}
					break;
					
				case 1:
					//接收消息类别
					msg_type = r_data;
					sumA += r_data;
					sumB += sumA;
					rc_step1 = 2;
					rc_step2 = 0;
					break;
				
				case 2:
					//接收消息长度
					if( r_data > MAX_SDI_PACKET_SIZE )
					{
						reset_SDI_RC;
						break;
					}
					msg_length = r_data;
					sumA += r_data;
					sumB += sumA;
					if( msg_length == 0 )
						rc_step1 = 4;
					else
						rc_step1 = 3;
					rc_step2 = 0;
					break;
					
				case 3:
					//接收数据包
					msg_pack[ rc_step2 ] = r_data;				
					sumA += r_data;
					sumB += sumA;
					++rc_step2;
					if( rc_step2 >= msg_length )
					{
						rc_step1 = 4;
						rc_step2 = 0;
					}
					break;
					
				case 4:
					//接收校验位
					if( rc_step2 == 0 )
					{
						if( sumA != r_data )
						{
							reset_SDI_RC;
							break;
						}
						rc_step2 = 1;
					}
					else
					{
						if( sumB == r_data )
						{
							for( uint16_t i = 4; i <= msg_length; i+=4 )
							{
								uint8_t p_ind = (i-4) >> 2;
								if( p_ind < 4 )
                {
                  user_data[p_ind] = *(float*)&msg_pack[i-4];
                }
							}
              reset_rc_connected();
						}
						reset_SDI_RC;
					}
					break;					
			}					
      
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&r_data, 1);
  }
}




