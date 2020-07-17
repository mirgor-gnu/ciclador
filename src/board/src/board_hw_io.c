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
//!   \file       board_hw_io.c
//!
//!   \brief      This is the code file for board hw io module.
//!
//!               This module defines the macros for setting the hw
//!               IOs.
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

//********************************************************************
//! \addtogroup high
//! @{
//!   \addtogroup sub
//!   @{
//********************************************************************
// include local module header here
#include "board_hw_io.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************


//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

#define X(a,f,b,c,d,e,g) {b,c,e,g,d},
hw_io_cfg_t const hw_io_cfg[] =
{
      IO_CFG_TABLE
};
#undef X


//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Function Definitions
//********************************************************************


//********************************************************************
//
//! IO pin configuration
//!
//! \param None.
//!
//! Initializes any IO pin.
//!
//! \return None.
//
//********************************************************************
void IO_Config(GPIO_TypeDef* GPIOx, uint16_t pin, uint32_t mode, uint32_t pull, uint32_t speed)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /*Configure GPIO pins : greenLed_Pin motorDirA_Pin motorDirB_Pin */
   GPIO_InitStruct.Pin = pin;
   GPIO_InitStruct.Mode = mode;
   GPIO_InitStruct.Pull = pull;
   GPIO_InitStruct.Speed = speed;
   HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}

//********************************************************************
//
// Close the Doxygen group.
//!   @}
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

