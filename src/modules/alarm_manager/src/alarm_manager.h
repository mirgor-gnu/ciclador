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
//!   \file       alarm_manager.h
//!
//!   \brief      Alarm Manager header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       07 May 2020
//!
//********************************************************************

#ifndef  _ALARM_MANAGER_H
#define  _ALARM_MANAGER_H 1

//********************************************************************
//! @addtogroup alarm_manager_imp
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
/**
 * Alarm status.
 * These values are used to represent the current status of each alarm
 */
typedef enum alarm_mgr_status_type_tag {
   AM_STATUS_OFF,       /**< OFF: Alarm disable */
   AM_STATUS_LATCHED,   /**< LATCHED: Alarm is informed to the user with display, buzzer, led and panel light */
   AM_STATUS_QUEUED,    /**< QUEUED: Alarm is queue because there is another alarm latched */
   AM_STATUS_SILENCED,  /**< SILENCED: Latched alarm silenced by the user */
} AlarmMgrStatusType;

/**
 * Alarm priority.
 * These values are used to represent the priority level of each alarm.
 * The manager has implemented 3 levels of priority.
 */
typedef enum alarm_mgr_priority_type_tag {
   AM_PRIORITY_HIGH,    /**< HIGH: Most priority */
   AM_PRIORITY_MIDDLE,  /**< MIDDLE: Middle priority */
   AM_PRIORITY_LOW,     /**< LOW: Less priority */
} AlarmMgrPriorityType;

/**
 * HMI Motor control modes
 * VentilatorMgr control modes referenced in this module so the user
 * could select between them.
 */
typedef enum alarm_mgr_queue_status_tag
{
   AM_EMPTY_QUEUE,       /**< Ventilator runs using tidal volume control */
   AM_NOT_EMPTY_QUEUE,   /**< Ventilator runs using pressue control */
} AlarmMgrQueueStatusType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _ALARM_MANAGER_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

