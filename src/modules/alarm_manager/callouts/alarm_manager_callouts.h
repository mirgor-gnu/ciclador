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
//!   \file       alarm_manager_callouts.h
//!
//!   \brief      Alarm manager callouts header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       06 May 2020
//!
//********************************************************************

#ifndef  _ALARM_MANAGER_CALLOUTS_H
#define  _ALARM_MANAGER_CALLOUTS_H 1

//********************************************************************
//! @addtogroup alarm_manager_callouts
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
 * Set alarm screen.
 * This function is called by the #AlarmManager to update the display
 * screen because there is a new latched alarm or it was silenced.
 *
 * @param id alarm id to show on the display
 * @param status of the display
 * @param errorValue value that causes the alarm situation and it will
 * be shown on the display
 */
void AlarmMgr_SetAlarmScreen(AlarmIdType id, bool status, uint32_t errorValue, AlarmMgrQueueStatusType queue);

/**
 * Updates the silenced alarm counter every time a new alarm is added
 * or removed from this status.
 *
 * @param counter Quantity of silenced alarms
 */
void AlarmMgr_UpdateSilencedAlarmCounter(uint32_t counter);

#endif // _ALARM_MANAGER_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

