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
//!   \file       hmi_api.h
//!
//!   \brief      HMI APIs header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//!
//********************************************************************

#ifndef  _HMI_API_H
#define  _HMI_API_H 1

#include "alarm_manager_api.h"

//********************************************************************
//! @addtogroup hmi_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
/**
 * HMI Ventilator states
 * The ventilator state is needed to handle the #LED_RUN and enables
 * the possibilty to change the control mode.
 */
typedef enum hmi_ventilator_state_tag
{
   HMI_VENTILATOR_STATE_IDLE,
   HMI_VENTILATOR_STATE_CYCLING,
} HmiVentilatorStateType;

/**
 * HMI Motor control modes
 * VentilatorMgr control modes referenced in this module so the user
 * could select between them.
 */
typedef enum hmi_motor_control_tag
{
   HMI_MOTOR_CONTROL_VOLUME,     /**< Ventilator runs using tidal volume control */
   HMI_MOTOR_CONTROL_PRESSURE,   /**< Ventilator runs using pressue control */
} HmiMotorControlType;

/**
 * HMI Motor control modes
 * VentilatorMgr control modes referenced in this module so the user
 * could select between them.
 */
typedef enum hmi_alarm_queue_status_tag
{
   HMI_ALARM_SCREEN_EMPTY_QUEUE,       /**< Ventilator runs using tidal volume control */
   HMI_ALARM_SCREEN_NOT_EMPTY_QUEUE,   /**< Ventilator runs using pressue control */
} HmiAlarmQueueStatusType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier

/**
 * Initializes the HMI module.
 * This function shall be called before call any other API function
 *
 * @return #E_OK if initialization is successful\n
 *         #E_ERROR is initialization fails
 */
extern StatusType Hmi_Init(void);

/**
 * Set or reset an alarm screen.
 * This function shall be called to enable and disable an alarm screen.
 *
 * @param id #AlarmIdType to configure
 * @param status true or false to enable/disable the alarm screen 
 * @param errorValue depends of each alarm. If the alarm does not have
 * any value to show, this parameter could be any number.
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_SetAlarm(AlarmIdType alarmId, Bool status, uint32_t errorValue, HmiAlarmQueueStatusType queue);

/**
 * Set silenced alarms quantity.
 * It updates the quantity of silenced alarms to display it on the second
 * main screen.
 *
 * @param counter quantity of silenced alarms. Actually this number must be between
 * 0 and 9.
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_SetSilencedAlarms(uint32_t counter);

/**
 * Setter of the PEEP parameter
 * 
 * @param peep new value
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_UpdatePEEP(uint32_t peep);

/**
 * Setter of the PIP parameter
 *
 * @param peep new value
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_UpdatePIP(uint32_t pip);

/**
 * Setter of the PPlt parameter
 *
 * @param pplt new value
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_UpdatePPlt(uint32_t pplt);

/**
 * Setter of the Tidal Volume parameter
 *
 * @param tidalVol new value
 *
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_UpdateTidalVolume(uint32_t tidalVol);

/**
 * Setter of the Ventilator status
 *
 * @param newState to turn on or off the #LED_RUN and allows the
 * control mode change
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_UpdateVentilatorState(HmiVentilatorStateType newState);

/**
 * Receives a new KeyMap.
 * This function shall be called by the Keyboard Driver to inform
 * a KeyMap where each active bit represent one button.
 *
 * @param keyMap each bit represent a different button
 * @param type #LKP or #SKP defined by the Keyboard Driver
 * 
 * @return #E_OK if module is initialized\n
 *         #E_ERROR API called before module init
 */
extern StatusType Hmi_NewKeyMap(uint32_t keyMap, uint8_t type);

#endif // _HMI_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

