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
//!   \file       usart_drv_api.h
//!
//!   \brief      usart drive APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

#ifndef  _USART_DRV_API_H
#define  _USART_DRV_API_H 1


//********************************************************************
//! @addtogroup usart_drv_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier
/**
 * @brief Initialize the usrt driver module.
 *        - Initialize the local ring buffer to process the information
 *        - Initialize hardware USART as UART 
 *        - Initialize the DMA to work with the UART
 *  
 * @param none
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType USARTDrv_Init(void);

/**
 * @brief Send data through the USART.
 *        This is a non blocking function
 *  
 * @param pData pointer to the data to send
 * @param pData size of the data to send
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType USARTDrv_Send(void *pData, uint32_t size);

/**
 * @brief Takes data from the Ring buffer and loads into
 *        pData. 
 *        This function does not interacts with the DMA or
 *        USART hardware
 *  
 * @param pData pointer to where the received data should be written
 * @param pData size of the data to receive
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType USARTDrv_Receive(void *pData, uint32_t size);

/**
 * @brief Periodic function, if there is data present in the 
 *        ring buffer, it calls a callout to process it.
 *  
 * @param none
 *
 * @return none
 */
extern void USARTDrv_Update(void);

/**
 * @brief Interrupt handler of the USART, it copies the information
 *        received from the DMA into the ring buffer.
 *  
 * @param none
 *
 * @return none
 */
extern void USARTDrv_IRQHandler(void);

/**
 * @brief Interrupt handler of DMA Tx
 *  
 * @param none
 *
 * @return none
 */
extern void USARTDrv_DMATxIRQHandler(void);

/**
 * @brief Interrupt handler of DMA Tx
 *  
 * @param none
 *
 * @return none
 */
extern void USARTDrv_DMARxIRQHandler(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _USART_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

