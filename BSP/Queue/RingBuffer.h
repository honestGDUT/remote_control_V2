/****************************************************************************/
/**
* @file RingBuffer.h
*****************************************************************************/

#ifndef _RING_BUFFER_H_      
#define _RING_BUFFER_H_

/***************************** Include Files ********************************/
//#include "common_def.h"
#include <stdint.h>
/************************** Constant Definitions *****************************/

/* do note that we should reserve 1 byte data to avoid mistake of data full and empty 
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * | 7 | 8 |   |   |   |   |   |   | 1 | 2 | 3 | 4 | 5 | 6 |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * so if we want a 1024 buffer, we should define size as 1024+1		                                    
 */
#define RING_BUFFER_SIZE    (1024+1)
#define RING_BUFFER_ELEM_TYPE  uint8_t

#define USE_MEMCPY_PERF   1
/**************************** Type Definitions *******************************/
typedef struct{    
    uint8_t*   writePtr;         //buffer pointer to be added.
    uint8_t*   readPtr;          //buffer pointer to be read and store into flash.
    uint8_t*   endPtr;           //constant var to store buffer end pointer.
    uint8_t    bufferData[RING_BUFFER_SIZE];
} RING_BUFFER_T;

/************************** Functions Definitions******************************/
int ringBuffer_int(RING_BUFFER_T* pbuffer);
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
int ringBuffer_get_used_size(RING_BUFFER_T* pbuffer);
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
int ringBuffer_get_unused_size(RING_BUFFER_T* pbuffer);
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
int ringBuffer_push_data(RING_BUFFER_T* pbuffer, uint8_t* pdata, int data_size);  //push into a data.
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
int ringBuffer_pop_data(RING_BUFFER_T* pbuffer, uint8_t* pdata, int data_size);  //pop out a data

#endif
