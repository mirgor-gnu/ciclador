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
//!   \file       alarm_manager_api.h
//!
//!   \brief      Alarm Manager APIs header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       06 May 2020
//!
//********************************************************************

#ifndef  _ALARM_MANAGER_API_H
#define  _ALARM_MANAGER_API_H 1

#include "alarm_manager_conf.h"

//********************************************************************
//! @addtogroup alarm_manager_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

#undef X
 #define X(a, b, c, d) a,
 /**
  * Alarm Id.
  * All the available alarms.
  * The #AlarmIdType must be used by other modules to interact with 
  * this manager.
  */
 typedef enum {
    AM_ALARMS_CFG
    AM_ALARMS_MAX,
    AM_ALARM_NONE,
    AM_ALARMS_UNUSED_CFG
 } AlarmIdType;

#undef X
 #define X(a) a,
 typedef enum {
    AM_INTERNAL_FAILURE_CFG
 } InternalFailureType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier
/**
 * Initializes the Alarm Manager module.
 * This function shall be called before call any other API function
 *
 * @return #E_OK if initialization is successful\n
 *         #E_ERROR is initialization fails
 */
extern StatusType AlarmMgr_Init(void);

/**
 * Module periodic update.
 * This function shall be called at fixed time intervals. The Alarm Manager
 * internal state is updated with this function call.
 */
extern void AlarmMgr_Update(void);

/**
 * Set or reset an alarm.
 * This function shall be called to enable and disable an alarm.
 *
 * @param id alarm id to configure
 * @param status of the alarm 
 * @param errorValue depends of each alarm. If the alarm does not have
 * any value to show, this parameter could be any number.
 *
 * @return #E_OK if the id is correct\n
 *         #E_ERROR if the id is not valid
 */
extern StatusType AlarmMgr_SetAlarm(AlarmIdType id, bool status, void * errorValue);

/**
 * Silence a latched alarm.
 * This function shall be called from HMI module when the user silence
 * the latched alarm.
 */
extern StatusType AlarmMgr_Silence(void);

/**
 * Pause latched alarm audio.
 * This function shall be called from HMI module when the user pause
 * the latched alarm audio.
 */
extern StatusType AlarmMgr_PauseAudio(void);

/**
 * Debug function to print actual state of internal Alarm Manager
 * variables using logger module.
 */
extern void AlarmMgr_GetSilencedAlarms(uint32_t * alarmVector, uint32_t * alarmQuantity);

/**
 * Debug function to print actual state of internal Alarm Manager
 * variables using logger module.
 */
extern void AlarmMgr_AlarmStatus(void);

#endif // _ALARM_MANAGER_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

