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
//!   \file       motor_manager_callouts.h
//!
//!   \brief      motor manager callouts header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       23 Apr 2020
//!
//********************************************************************

#ifndef  _MOTOR_MANAGER_CALLOUTS_H
#define  _MOTOR_MANAGER_CALLOUTS_H 1

//********************************************************************
//! @addtogroup motor_manager_callouts
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

/**
 * @brief Callout to bind the motor manager FSM with the actions at the
 *        hardware level, in this case the motor driver FSM
 *  
 * @param state the current state of the motor manager FSM
 * @param driveLevel the speed of the movement needed
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorManager_SetMotorState(MotorStateType state, uint32_t driveLevel);

/**
 * @brief UNUSED (this information is gathered by motor driver FSM now) - 
 *        Callout from the lower levels to get the current position
 *  
 * @param none
 *
 * @return the current position
 */
extern int32_t MotorManager_getMotorPosition(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _MOTOR_MANAGER_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

