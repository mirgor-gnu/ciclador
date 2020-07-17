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
//!   \file       usart_drv_conf.h
//!
//!   \brief      usart driver configuration header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

#ifndef  _USART_DRV_CONF_H
#define  _USART_DRV_CONF_H 1

//********************************************************************
//! @addtogroup usart_drv_conf
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define USART_DRV_USART             1                       /**< Number of the usart used (USART1)*/
//#define USART_DRV_IRQ_NAME          (USART1_IRQn)
#define USART_DRV_DMA_CONTROLLER    1 //(DMA1)              /**< Number of the DMA controller used (DMA1)*/
#define USART_DRV_DMA_TX_CHANNEL    4                       /**< Channel of the DMA used by the usart TX (DMA1_Channel4)*/
#define USART_DRV_DMA_RX_CHANNEL    5                       /**< Channel of the DMA used by the usart RX (DMA1_Channel5)*/
#define USART_DRV_DMA_IRQ_PRIORITY  (4)                     /**< DMA interrupt priority */
#define USART_DRV_BAUD_RATE         (2*500000)              /**< USART baudrate */
#define USART_DRV_WORDLENGTH        (UART_WORDLENGTH_8B)    /**< USART word length */    
#define USART_DRV_STOPBITS          (UART_STOPBITS_1)       /**< USART stop bits */
#define USART_DRV_PARITY            (UART_PARITY_NONE)      /**< USART parity */
#define USART_DRV_MODE              (UART_MODE_TX_RX)       /**< USART mode */
#define USART_DRV_HWCONTROL         (UART_HWCONTROL_NONE)   /**< USART flow control */
#define USART_DRV_OVERSAMPLING      (UART_OVERSAMPLING_16)  /**< USART oversampling value */

#define USART_DRV_RX_BUFFER_SIZE    (64)                    /**< USART rx buffer size */

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _USART_DRV_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

