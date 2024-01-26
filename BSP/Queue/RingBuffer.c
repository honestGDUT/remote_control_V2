/******************************************************************************/
/*****************************************************************************/
/**
*
* @file RingBuffer.c
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* 1.00  kahn.qiao  25/06/2023 Fisrt edit.
* 2.00  kahn.qiao  29/12/2023 to support memcpy performance.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "RingBuffer.h"
#include <stdlib.h>
#include "string.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/


/************************** Function Prototypes ******************************/
int ringBuffer_int(RING_BUFFER_T* pbuffer)
{
    pbuffer->writePtr = pbuffer->bufferData;
    pbuffer->readPtr =  pbuffer->bufferData;
    pbuffer->endPtr =  pbuffer->bufferData + RING_BUFFER_SIZE;
    return 0;
}
/*****************************************************************************/
/**
*
* get Ring buffer used data size,
* 
* 
* @param    pbuffer  RING_BUFFER_T*    Ring buffer struct pointer.
* @return   int    Ring buffer used data size.
*
* @note     None.
*
******************************************************************************/
int ringBuffer_get_used_size(RING_BUFFER_T* pbuffer)
{
    if(pbuffer->writePtr>=pbuffer->readPtr)
    {
        return pbuffer->writePtr - pbuffer->readPtr;
    }else
    {
        return pbuffer->endPtr - pbuffer->readPtr + pbuffer->writePtr - pbuffer->bufferData;
    }
}
/*****************************************************************************/
/**
*
* get Ring buffer available to use(unused) data size,
* 
* 
* @param    pbuffer  RING_BUFFER_T*    Ring buffer struct pointer.
* @return   int    Ring buffer unused data size.
*
* @note     None.
*
******************************************************************************/
int ringBuffer_get_unused_size(RING_BUFFER_T* pbuffer)
{
    return RING_BUFFER_SIZE - ringBuffer_get_used_size(pbuffer)-1;
}
/*****************************************************************************/
/**
*
*  push a data into Ring buffer,
* 
* 
* @param    pbuffer     RING_BUFFER_T*    Ring buffer struct pointer.
* @param    pdata       uint8_t*          data to be added into Ring buffer.
* @param    data_size   int               data to be added into Ring buffer.
* @return   int         data size actuality pushed into Ring buffer from pdata.
*
* @note     None.
*
******************************************************************************/
int ringBuffer_push_data(RING_BUFFER_T* pbuffer, uint8_t* pdata, int data_size)  //push into a data.
{
    int i,avail_len,index,write_len;
    avail_len = ringBuffer_get_unused_size(pbuffer);
    if(avail_len < data_size)  // to be write data size > buffer availiale size.
    {
        return 0;
    }else
    {
#if USE_MEMCPY_PERF		
		if(data_size <= (pbuffer->endPtr - pbuffer->writePtr))
		{
			/* write data to ring buffer 
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 * |   |   |   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |   |   |   |
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 *			                                    
			 */
			memcpy(pbuffer->writePtr,
			   pdata,
			   data_size);
		}else
		{
			/* write data to ring buffer
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 * | 7 | 8 |   |   |   |   |   |   | 1 | 2 | 3 | 4 | 5 | 6 |
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 *			                                    
			 */
			write_len = pbuffer->endPtr - pbuffer->writePtr;
			memcpy(pbuffer->writePtr,  //from the write position.
			   pdata,
			   write_len);
			memcpy(pbuffer->bufferData, //from the start of buffer.
			   pdata+write_len,
			   data_size - write_len);   
		}		
#else		
        index = pbuffer->writePtr - pbuffer->bufferData;
        for(i=0;i<data_size;i++)
        {
            pbuffer->bufferData[index] = (uint8_t)(pdata[i]);
            index = (index +1) %RING_BUFFER_SIZE;
        }
#endif
        pbuffer->writePtr += data_size;
        if(pbuffer->writePtr>=pbuffer->endPtr) //out of buffer
        {
            pbuffer->writePtr -= RING_BUFFER_SIZE;
        }
		
        return data_size;
    }
}
/*****************************************************************************/
/**
*
*  pop data from Ring buffer,
* 
* 
* @param    pbuffer     RING_BUFFER_T*    Ring buffer struct pointer.
* @param    pdata       uint8_t*          data buffer to be popped from Ring buffer.
* @param    data_size   int               data to be popped from Ring buffer.
* @return   int         data size actuality popped from Ring buffer to pdata.
*
* @note     None.
*
******************************************************************************/
int ringBuffer_pop_data(RING_BUFFER_T* pbuffer, uint8_t* pdata, int data_size)  //pop out a data
{
    int i,avail_len,index,read_len;
    avail_len = ringBuffer_get_used_size(pbuffer);
	
	avail_len = (avail_len<data_size)?avail_len:data_size;
	if(avail_len>0)
    {
        
#if USE_MEMCPY_PERF			
		if(avail_len <= (pbuffer->endPtr - pbuffer->readPtr))
		{
			/* read data from ring buffer 
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 * |   |   |   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |   |   |   |
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 *			                                    
			 */
			memcpy(pdata,
			   pbuffer->readPtr,
			   avail_len);
		}else
		{
			/* read data from ring buffer 
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 * | 7 | 8 |   |   |   |   |   |   | 1 | 2 | 3 | 4 | 5 | 6 |
			 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			 *			                                    
			 */
			read_len = pbuffer->endPtr - pbuffer->readPtr;
			memcpy(pdata,  //from the read position.
			   pbuffer->readPtr,
			   read_len);
			memcpy(pdata+read_len, //from the start of buffer.
			   pbuffer->bufferData,
			   avail_len - read_len);   
		}
#else
		index = pbuffer->readPtr - pbuffer->bufferData;
        for(i=0;i<avail_len;i++)
        {
            pdata[i] = pbuffer->bufferData[index];
            index = (index +1) %RING_BUFFER_SIZE;
        }	
#endif	
        pbuffer->readPtr += avail_len;
        if(pbuffer->readPtr>=pbuffer->endPtr) //out of buffer
        {
            pbuffer->readPtr -= RING_BUFFER_SIZE;
        }
        return avail_len;
    }
}

