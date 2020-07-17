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
//!   \file       power_manager_callouts_imp.c
//!
//!   \brief      This is the power manager callouts implementation.
//!
//!   \author     Esteban Pupillo
//!
//!   \date       11 MAY 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "logger_api.h"
#include "alarm_manager_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "power_manager_api.h"
#include "power_manager_conf.h"
#include "power_manager_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG   "PM"

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
void PowerMgr_OnStateChange(PowerMgrStateType newState)
{
   static Bool alarmInformed = FALSE;

   LOG_PRINT_INFO(DEBUG_PM, LOG_TAG, "s=%lu;", newState);

   if((POWER_MGR_POWER_STATE_NORMAL == newState) && (alarmInformed))
   {
      // we have to clear the alarm
      AlarmMgr_SetAlarm(AM_BATTERY_MODE, FALSE, NULL);
      alarmInformed = FALSE;
   }

   if((POWER_MGR_POWER_STATE_BACKUP == newState) && (!alarmInformed))
   {
      // we have to inform the alarm
      AlarmMgr_SetAlarm(AM_BATTERY_MODE, TRUE, NULL);
      alarmInformed = TRUE;
   }

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

