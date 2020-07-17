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
//!   \file       rotary_enc_drv_conf.h
//!
//!   \brief      rotary encoder driver configuration header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

#ifndef  _ROTARY_ENC_DRV_CONF_H
#define  _ROTARY_ENC_DRV_CONF_H 1

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
//! @addtogroup rotary_enc_drv_conf
//!   @{
//********************************************************************

// encoder pin & port definition
#define ROTARY_ENC_DRV_INPUT_A      (IO_MOTOR_ENCA)         /**< GPIO pin used by the first encoder  */
#define ROTARY_ENC_DRV_INPUT_B      (IO_MOTOR_ENCB)         /**< GPIO pin used by the first encoder  */
#define ROTARY_ENC_DRV_IRQ          (EXTI15_10_IRQn)        /**< GPIO interrupt callback used by both encoder  */
#define ROTARY_ENC_DRV_IRQ_PRIORITY (0)                     /**< GPIO interrupt priority used by both encoder  */

// encoder parameters
#define ROTARY_ENC_DRV_PPR              (600)               /**<  Encoder pulse per revolution value, used to calculate the angular distance travelled for each pulse */

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

#endif // _ROTARY_ENC_DRV_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

