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
//!
//!   \file       adc_drv_conf.h
//!
//!   \brief      ADC driver configuration header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _ADC_DRV_CONF_H
#define  _ADC_DRV_CONF_H 1

//********************************************************************
//! @addtogroup adc_drv_conf
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define ADC_DRV_DMA_CHANNEL            (DMA1_Channel1)               /**< DMA channel used */
#define ADC_DRV_DMA_IRQ_NAME           (DMA1_Channel1_IRQn)          /**< DMA channel interrupt number */
#define ADC_DRV_DMA_IRQ_PRIORITY       (2)                           /**< DMA channel interrupt priority */
#define ADC_DRV_MAX_OUTPUT             (4095)                        /**< Max count number of conversion with 12 bits */
#define ADC_VOLTAGE_REFERENCE          (3.3)                         /**< Voltage reference */
#define ADC_DRV_STATS_AVG_MAX_WINDOW   (2048)                        /**< Average window max size in samples */
#define ADC_DRV_MAX_TRIGGERS           (10)                          /**< Max number of triggers of the system */
#define ADC_DRV_WAIT_FOR_LOCK_TIMEOUT  (2U)                          /**< Timeout to wait to lock the value */

/**
 * The configured ADC channels in the system
 */
#define ADC_DRV_INPUTS_CFG \
   X(AIN_PRESSURE        , ADC_CHANNEL_4  , -90, 1155  )  \
   X(AIN_M1_CURRENT      , ADC_CHANNEL_10 ,   0, 33000 )  \
   X(AIN_CH2             , ADC_CHANNEL_6  , -90, 1155 )  \

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _ADC_DRV_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

