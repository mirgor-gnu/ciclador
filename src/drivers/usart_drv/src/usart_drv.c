/*
 *
 * MIT License
 *
 * Copyright (c) 2020 Mirgor
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//********************************************************************
//
//!   \file       usart_drv.c
//!
//!   \brief      This is the usart driver implementation file.
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "ringbuf.h"

//********************************************************************
//! @addtogroup usart_drv_imp
//!   @{
//********************************************************************

#include "usart_drv_conf.h"
#include "usart_drv_api.h"
#include "usart_drv_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define USART_INSTANCE_(num)  USART##num
#define USART_INSTANCE(num)   USART_INSTANCE_(num)

#define USART_IRQ_(num)  USART##num##_IRQn
#define USART_IRQ(num)   USART_IRQ_(num)

#define DMA_CHANNEL_(dma,ch)  DMA##dma##_Channel##ch
#define DMA_CHANNEL(dma,ch)   DMA_CHANNEL_(dma,ch)

#define DMA_IRQ_(dma,ch) DMA##dma##_Channel##ch##_IRQn
#define DMA_IRQ(dma,ch)  DMA_IRQ_(dma,ch)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct usart_data_tag
{
   UART_HandleTypeDef huart;
   DMA_HandleTypeDef hdma_usart_tx;
   DMA_HandleTypeDef hdma_usart_rx;

   uint8_t rx_buffer[USART_DRV_RX_BUFFER_SIZE];
   Ring_Buf_Type rx_ring_buf;

   uint8_t rx_dma_buffer[USART_DRV_RX_BUFFER_SIZE];
   uint32_t rx_dma_writeIdx;
} USARTDrvType;
//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static StatusType usart_drv_peripheral_init(void);
static void usart_drv_rx_check(void);
static void usart_drv_rx_process(uint32_t readIdx, uint32_t size);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static USARTDrvType usart_data;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType USARTDrv_Init(void)
{
   StatusType err;

   //init rx ring buffer
   RingBufInit(&usart_data.rx_ring_buf, usart_data.rx_buffer, NULL, USART_DRV_RX_BUFFER_SIZE);
   usart_data.rx_dma_writeIdx = 0;

   //init usart peripheral
   err = usart_drv_peripheral_init();

   //issue a read command to start dma operation
   //since we have configured a circular DMA the dma operation will never stop
   HAL_UART_Receive_DMA(&usart_data.huart, usart_data.rx_dma_buffer, USART_DRV_RX_BUFFER_SIZE);

   return err;

}

void USARTDrv_Update(void)
{
   if (!RingBufEmpty(&usart_data.rx_ring_buf))
   {
      USARTDrv_OnReceiveComplete(&usart_data.huart, RingBufUsed(&usart_data.rx_ring_buf));
   }
}

StatusType USARTDrv_Send(void *pData, uint32_t size)
{
   return HAL_UART_Transmit_DMA(&usart_data.huart, pData, size);
}

StatusType USARTDrv_Receive(void *pData, uint32_t size)
{
   if (NULL == pData)
   {
      return E_ERROR;
   }

   // check if we have enough data
   if (size > RingBufUsed(&usart_data.rx_ring_buf))
   {
      return E_ERROR;
   }
   // copy data
   RingBufRead(&usart_data.rx_ring_buf, (uint8_t*)pData, (uint16_t)size);

   return E_OK;
}

void USARTDrv_IRQHandler(void)
{
   if (__HAL_UART_GET_FLAG(&usart_data.huart, UART_FLAG_IDLE) != RESET)
   {
      __HAL_UART_CLEAR_IDLEFLAG(&usart_data.huart);
      usart_drv_rx_check();
   }
   HAL_UART_IRQHandler(&usart_data.huart);
}

void USARTDrv_DMATxIRQHandler(void) {
   HAL_DMA_IRQHandler(&usart_data.hdma_usart_tx);
}

void USARTDrv_DMARxIRQHandler(void) {
   HAL_DMA_IRQHandler(&usart_data.hdma_usart_rx);
}


static StatusType usart_drv_peripheral_init(void)
{
  // init uart peripheral
  usart_data.huart.Instance = USART_INSTANCE(USART_DRV_USART);
  usart_data.huart.Init.BaudRate = USART_DRV_BAUD_RATE;
  usart_data.huart.Init.WordLength = USART_DRV_WORDLENGTH;
  usart_data.huart.Init.StopBits = USART_DRV_STOPBITS;
  usart_data.huart.Init.Parity = USART_DRV_PARITY;
  usart_data.huart.Init.Mode = USART_DRV_MODE;
  usart_data.huart.Init.HwFlowCtl = USART_DRV_HWCONTROL;
  usart_data.huart.Init.OverSampling = USART_DRV_OVERSAMPLING;
  if (HAL_UART_Init(&usart_data.huart) != HAL_OK)
  {
    return E_ERROR;
  }

  // enable IDLE interrupt
  // we use this interrupt to fire a new dma transaction
  __HAL_UART_ENABLE_IT(&usart_data.huart, UART_IT_IDLE);

  // USART DMA Init
  // USART_TX Init
  usart_data.hdma_usart_tx.Instance = DMA_CHANNEL(USART_DRV_DMA_CONTROLLER,USART_DRV_DMA_TX_CHANNEL);
  usart_data.hdma_usart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  usart_data.hdma_usart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  usart_data.hdma_usart_tx.Init.MemInc = DMA_MINC_ENABLE;
  usart_data.hdma_usart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  usart_data.hdma_usart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  usart_data.hdma_usart_tx.Init.Mode = DMA_NORMAL;
  usart_data.hdma_usart_tx.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&usart_data.hdma_usart_tx) != HAL_OK)
  {
     return E_ERROR;
  }

  // USART_RX Init
  usart_data.hdma_usart_rx.Instance = DMA_CHANNEL(USART_DRV_DMA_CONTROLLER,USART_DRV_DMA_RX_CHANNEL);
  usart_data.hdma_usart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  usart_data.hdma_usart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  usart_data.hdma_usart_rx.Init.MemInc = DMA_MINC_ENABLE;
  usart_data.hdma_usart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  usart_data.hdma_usart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  usart_data.hdma_usart_rx.Init.Mode = DMA_CIRCULAR;
  usart_data.hdma_usart_rx.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&usart_data.hdma_usart_rx) != HAL_OK)
  {
     return E_ERROR;
  }

  __HAL_LINKDMA(&usart_data.huart,hdmatx, usart_data.hdma_usart_tx);
  __HAL_LINKDMA(&usart_data.huart,hdmarx, usart_data.hdma_usart_rx);

  // USART interrupt Init
  HAL_NVIC_SetPriority(USART_IRQ(USART_DRV_USART), USART_DRV_DMA_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(USART_IRQ(USART_DRV_USART));

  //DMA RX interrupt configuration
  HAL_NVIC_SetPriority(DMA_IRQ(USART_DRV_DMA_CONTROLLER, USART_DRV_DMA_RX_CHANNEL), 0, 0);
  HAL_NVIC_EnableIRQ(DMA_IRQ(USART_DRV_DMA_CONTROLLER, USART_DRV_DMA_RX_CHANNEL));

  // DMA TX interrupt configuration
  HAL_NVIC_SetPriority(DMA_IRQ(USART_DRV_DMA_CONTROLLER, USART_DRV_DMA_TX_CHANNEL), 0, 0);
  HAL_NVIC_EnableIRQ(DMA_IRQ(USART_DRV_DMA_CONTROLLER, USART_DRV_DMA_TX_CHANNEL));

  return E_OK;
}


static void usart_drv_rx_check(void)
{
   uint32_t bytesRcvd,newWriteIdx;

   // read how many bytes were actually received
   bytesRcvd = __HAL_DMA_GET_COUNTER(&usart_data.hdma_usart_rx);

   if (0 == bytesRcvd)
      return;

   // Calculate current position in buffer
   newWriteIdx = USART_DRV_RX_BUFFER_SIZE - bytesRcvd;
   // check if we have received something new
   if (newWriteIdx != usart_data.rx_dma_writeIdx) {
      // since we are using a circular buffer for the DMA we need to sync our pointers
      if (newWriteIdx > usart_data.rx_dma_writeIdx) {
         // Current position is over previous one. We are in "linear" mode
         // Process data directly by subtracting "pointers"
         usart_drv_rx_process(usart_data.rx_dma_writeIdx, newWriteIdx - usart_data.rx_dma_writeIdx);
      } else {
         // We are in "overflow" mode
         // First process data to the end of buffer
         usart_drv_rx_process(usart_data.rx_dma_writeIdx, USART_DRV_RX_BUFFER_SIZE - usart_data.rx_dma_writeIdx);
         // Check and continue with beginning of buffer
         if (newWriteIdx > 0) {
            usart_drv_rx_process(0, newWriteIdx);
         }
      }
   }
   //update our write index
   usart_data.rx_dma_writeIdx = newWriteIdx;
   if (USART_DRV_RX_BUFFER_SIZE == usart_data.rx_dma_writeIdx) {
      usart_data.rx_dma_writeIdx = 0;
   }
}

static void usart_drv_rx_process(uint32_t readIdx, uint32_t size)
{
   if ((size == 0) || (USART_DRV_RX_BUFFER_SIZE <= size))
   {
      // error condition. we shold never be called with
      // zero size or a size higher than the buffer size
      return;
   }
   //copy received data to output buffer
   RingBufWrite(&usart_data.rx_ring_buf, &usart_data.rx_dma_buffer[readIdx], size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   if (huart->Instance == usart_data.huart.Instance)
   {
      USARTDrv_OnTransmitComplete(huart);
   }
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
   // do nothing for now
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if (huart->Instance == usart_data.huart.Instance)
   {
      usart_drv_rx_check();
   }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
   if (huart->Instance == usart_data.huart.Instance)
   {
      usart_drv_rx_check();
   }
}

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

