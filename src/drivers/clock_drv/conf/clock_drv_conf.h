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
//!   \file       clock_drv_conf.h
//!
//!   \brief      Clock driver configuration header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _CLOCK_DRIVER_CONF_H
#define  _CLOCK_DRIVER_CONF_H 1

//********************************************************************
//! @addtogroup clock_drv_conf
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define CLOCK_DRV_HIGH_RES_TIMER                (TIM1)                      /**< HardThe hardware timer used for the high resolution timer */
#define CLOCK_DRV_HIGH_RES_TIMER_PRESCALER      (72-1)                      /**< High resolution timer prescaler */
#define CLOCK_DRV_HIGH_RES_TIMER_CLK_DIV        (TIM_CLOCKDIVISION_DIV1)    /**< High resolution timer clock divisor */

#define CLOCK_DRV_TIMER_PERIOD                  (100)                       /**< High resolution timer period (us) */

#define CLOCK_DRV_HIGH_RES_TIMER_IRQ_NAME          (TIM1_UP_IRQn)           /**< High resolution timer irq */
#define CLOCK_DRV_HIGH_RES_TIMER_IRQ_PRIORITY      (0)                      /**< High resolution timer priority */
#define CLOCK_DRV_HIGH_RES_TIMER_CC_IRQ_NAME       (TIM1_CC_IRQn)           /**< High resolution timer capture compare irq */
#define CLOCK_DRV_HIGH_RES_TIMER_CC_IRQ_PRIORITY   (2)                      /**< High resolution timer capture compare priority */

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

#endif // _CLOCK_DRIVER_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

