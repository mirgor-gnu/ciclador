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
//!   \file       motor_manager_callouts_imp.c
//!
//!   \brief      This is the motor manager callouts implementation.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       24 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "motor_drv_api.h"
#include "rotary_enc_drv_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "motor_manager_conf.h"
#include "motor_manager_api.h"
#include "motor_manager_callouts.h"

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

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Function Definitions
//********************************************************************

inline StatusType MotorManager_SetMotorState(MotorStateType state, uint32_t driveLevel)
{
   StatusType err;

   switch(state)
   {
      case MOTOR_MGR_MOTOR_STOP:
         err = MotorDrv_Stop(MOTOR_STOP_NORMAL);
         break;
      case MOTOR_MGR_MOTOR_RUN_UP:
         err = MotorDrv_Start(MOTOR_DIR_CW, driveLevel);
         break;
      case MOTOR_MGR_MOTOR_RUN_DOWN:
         err = MotorDrv_Start(MOTOR_DIR_CCW, driveLevel);
         break;
      case MOTOR_MGR_MOTOR_BRAKE:
         err = MotorDrv_Stop(MOTOR_STOP_BRAKE);
         break;
      default:
         err = E_ERROR;
         break;
   }

   return err;
}

inline int32_t MotorManager_getMotorPosition(void)
{
   return RotaryEncDrv_GetPosition();
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
