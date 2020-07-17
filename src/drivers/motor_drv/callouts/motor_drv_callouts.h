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
//!   \file       motor_drv_callouts.h
//!
//!   \brief      motor driver callouts header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _MOTOR_DRV_CALLOUTS_H
#define  _MOTOR_DRV_CALLOUTS_H 1

//********************************************************************
//! @addtogroup motor_drv_callouts
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * @brief Available source of errors of the motor
 * 
 */
typedef enum motor_drv_error_tag
{
   MOTOR_DRV_ERROR_NONE,               /**< No error */
   MOTOR_DRV_ERROR_HOMENOTFOUND,       /**< Home not found */
}MotorDrvErrorType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

/**
 * @brief Callout to get the current position of the motor.
 *        (For example from an encoder)
 *  
 * @param none
 *
 * @return the current position
 */
extern int32_t MotorDrv_GetPosition(void);

/**
 * @brief Callout to tell the current position is the 0
 *        (For example to an encoder)
 *  
 * @param none
 *
 * @return none
 */
extern void MotorDrv_ResetPosition(void);

/**
 * @brief Callout called when the motor reached the target position
 *  
 * @param none
 *
 * @return none
 */
extern void MotorDrv_OnMoveComplete(void);

/**
 * @brief Callout called when there is an error in the motor module
 *  
 * @param error the error found
 *
 * @return none
 */
extern void MotorDrv_OnError(MotorDrvErrorType error);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _MOTOR_DRV_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

