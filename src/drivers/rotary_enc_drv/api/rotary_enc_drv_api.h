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
//!   \file       rotary_enc_drv_api.h
//!
//!   \brief      rotary encoder driver APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

#ifndef  _ROTARY_ENC_DRV_API_H
#define  _ROTARY_ENC_DRV_API_H 1

//********************************************************************
//! @addtogroup rotary_enc_drv_api
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
 * @brief Initialize the rotary encoder module.
 *        - Initialize local variables
 *        - Initialize hardware GPIO pins and interrupt
 *  
 * @param none
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType RotaryEncDrv_Init(void);

/**
 * @brief Rotary encoder GPIO associated handler
 *        Calculate the time and steps since the last interrupt  
 * 
 * @param none
 *
 * @return none
 */
extern void RotaryEncDrv_IRQHandler(void);

/**
 * @brief Get the current position of the system
 * 
 * @param none
 *
 * @return The current position
 */
extern int32_t RotaryEncDrv_GetPosition(void);

/**
 * @brief Get the current position of the system. 
 *        Mainly used to set the position 0 as the current position
 *        to the rotary encoder module.
 * 
 * @param newPosition the new position to set as the current position
 *
 * @return none
 */
extern StatusType RotaryEncDrv_SetPosition(int32_t newPosition);

/**
 * @brief Get the current current speed of the system
 * 
 * @param none
 *
 * @return The speed of the system
 */
extern uint32_t RotaryEncDrv_GetSpeed(void);

/**
 * @brief UNUSED
 * 
 * @param none
 *
 * @return The period of the system
 */
extern uint32_t RotaryEncDrv_GetPeriod(void);

/**
 * @brief Periodic function to re-calculate the parameters using the 
 *        information obtained by the interruption between calls of this
 *        function.
 *        It updates the speed, steps counter, position and interval from 
 *        the last call
 * 
 * @param none
 *
 * @return none
 */
extern void RotaryEncDrv_Update(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _ROTARY_ENC_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

