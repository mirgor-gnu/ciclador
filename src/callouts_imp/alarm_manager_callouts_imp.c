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
//!   \file       alarm_manager_callouts_imp.c
//!
//!   \brief      This is the Alarm Manager callouts implementation.
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       12 May 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "hmi_api.h"

//********************************************************************
//! @addtogroup alarm_manager_callouts
//!   @{
//********************************************************************

#include "alarm_manager.h"
#include "alarm_manager_api.h"
#include "alarm_manager_conf.h"
#include "alarm_manager_callouts.h"

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
void AlarmMgr_SetAlarmScreen(AlarmIdType id, bool status, uint32_t errorValue, AlarmMgrQueueStatusType queue)
{
   Hmi_SetAlarm(id, status, errorValue,
		   	   (queue == AM_EMPTY_QUEUE) ? HMI_ALARM_SCREEN_EMPTY_QUEUE : HMI_ALARM_SCREEN_NOT_EMPTY_QUEUE);
}

void AlarmMgr_UpdateSilencedAlarmCounter(uint32_t counter)
{
   Hmi_SetSilencedAlarms(counter);
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
