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
//!   @file       alarm_manager_conf.h
//!
//!   @brief      Alarm manager configuration header file
//!
//!   @author     Agustin Diaz Antuna
//!
//!   @date       06 May 2020
//!
//********************************************************************

#ifndef  _ALARM_MANAGER_CONF_H
#define  _ALARM_MANAGER_CONF_H 1

//********************************************************************
//! @addtogroup alarm_manager_conf
//!   @{
//********************************************************************

#include "alarm_manager.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
/**
 * Enable more logs only for debug
 */
#define AM_DEBUG_SUPER_VERBOSE      (false)

/**
 * Disable buzzer only for debug
 */
#define AM_DEBUG_NO_BUZZER          (false)

/**
 * Update period in milliseconds
 */
#define ALARM_MGR_UPDATE_TIME_MS (5*2)

/**
 * Silenced alarm timeout in milliseconds
 */
#define ALARM_MGR_SILENCED_TIMEOUT_MS     (2*60*1000) // 2 min

/**
 * Silenced alarm timeout in cycles based on #ALARM_MGR_UPDATE_TIME_MS
 */
#define ALARM_MGR_SILENCED_TIMEOUT_CYCLES (ALARM_MGR_SILENCED_TIMEOUT_MS/ALARM_MGR_UPDATE_TIME_MS)

/**
 * Alarm led off
 */
#define AM_LED_BLINK_OFF         0

/**
 * Alarm led on
 */
#define AM_LED_BLINK_ON          1

/**
 * Alarm blink slow milliseconds period of 0.8 Hz
 */
#define AM_LED_BLINK_SLOW_MS     1250  // 0.8 Hz

/**
 * Alarm blink slow cycles based on #ALARM_MGR_UPDATE_TIME_MS
 */
#define AM_LED_BLINK_SLOW_CYCLES (AM_LED_BLINK_SLOW_MS/ALARM_MGR_UPDATE_TIME_MS)

/**
 * Alarm blink fast milliseconds period of 2.8 Hz
 */
#define AM_LED_BLINK_FAST_MS     357   // 2.8 Hz

/**
 * Alarm blink fast cycles based on #ALARM_MGR_UPDATE_TIME_MS
 */
#define AM_LED_BLINK_FAST_CYCLES (AM_LED_BLINK_FAST_MS/ALARM_MGR_UPDATE_TIME_MS)

/**
 * Alarm sound off
 */
#define AM_SOUND_OFF             0

/**
 * Alarm sound on
 */
#define AM_SOUND_ON              1

/**
 * Alarm beep sound milliseconds period
 */
#define AM_SOUND_INT_MS          357   // 2.8 Hz

/**
 * Alarm beep sound cycles based on #ALARM_MGR_UPDATE_TIME_MS
 */
#define AM_SOUND_INT_CYCLES      (AM_SOUND_INT_MS/ALARM_MGR_UPDATE_TIME_MS)

/**
 * Panel light pin ID
 */
#define AM_EXT_LIGHT             (IO_RELAY3)

/**
 * Alarm led pin ID
 */
#define AM_LED_ALARM             (IO_LED_ALARM_EN)

/**
 * Backup Battery led pin ID
 */
#define AM_LED_BACKUP_BATTERY    (IO_LED_BUB_EN)

/**
 * Alarm buzzer pin ID
 */
#define AM_BUZZER                (IO_BUZZ_L)

/**
 * Alarms setup.
 * It is used to configure each alarm PRIORITY, LED, LED blinking format,
 * sound format.
 * 
 * The ID defined here will be used by other modules to set the alarms and
 * show each alarm display message.
 * 
 * The input format is: X([id], [priority], [led], [led_format], [sound_format])
 */
#define AM_ALARMS_CFG \
   X(AM_AIRFLOW_ERROR             , AM_PRIORITY_HIGH  , AM_LED_BLINK_FAST_MS, AM_SOUND_ON     )  \
   X(AM_BATTERY_LOW               , AM_PRIORITY_HIGH  , AM_LED_BLINK_FAST_MS, AM_SOUND_ON     )  \
   X(AM_BATTERY_MODE              , AM_PRIORITY_HIGH  , AM_LED_BLINK_FAST_MS, AM_SOUND_ON     )  \
   X(AM_INTERNAL_FAILURE          , AM_PRIORITY_HIGH  , AM_LED_BLINK_FAST_MS, AM_SOUND_ON     )  \
   X(AM_PIP_NOTREACHED            , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_PIP_EXCEEDED              , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_UNDER_PRESSURE            , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_DRIVING_PRESSURE_LOW      , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_TIDAL_VOLUME_LOW          , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_TIDAL_VOLUME_HIGH         , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_INSPIRATORY_TIME_UNREACHED, AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_RESP_FREQ_NOT_MET         , AM_PRIORITY_MIDDLE, AM_LED_BLINK_SLOW_MS, AM_SOUND_ON     )  \
   X(AM_MAINTENANCE_REQUIRED      , AM_PRIORITY_LOW   , AM_LED_BLINK_ON     , AM_SOUND_INT_MS )  \

#define AM_ALARMS_UNUSED_CFG \
   X(AM_OVER_PRESSURE             , AM_PRIORITY_LOW   , AM_LED_BLINK_ON     , AM_SOUND_INT_MS )  \
   X(AM_BATTERY_VOLTAGE_LOW       , AM_PRIORITY_LOW   , AM_LED_BLINK_ON     , AM_SOUND_INT_MS )  \
   X(AM_DOOR_OPEN                 , AM_PRIORITY_LOW   , AM_LED_BLINK_ON     , AM_SOUND_INT_MS )  \

#define AM_INTERNAL_FAILURE_CFG \
   X(AM_IF_STACK_FAULT)          \
   X(AM_IF_MOTOR_HOME_NOT_FOUND) \
   X(AM_IF_MOTOR_ET_NOT_REACHED) \
   X(AM_IF_DFLOW_METER)          \

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _ALARM_MANAGER_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

