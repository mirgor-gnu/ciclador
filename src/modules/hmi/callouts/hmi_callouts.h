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
//!   \file       hmi_callouts.h
//!
//!   \brief      HMI callouts header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//!
//********************************************************************

#ifndef  _HMI_CALLOUTS_H
#define  _HMI_CALLOUTS_H 1

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
 * Convert KeyMap to a KeyId.
 *
 * @param keyMap where each active bit represents one button 
 * 
 * @return KeyIdType with the button pressed. If there is multiple
 * buttons detected, it returns #NO_KEY
 */
KeyIdType Hmi_ConvertKeyMapToKeyId(uint32_t keyMap);

/**
 * Getter of the silenced alarm list.
 *
 * @param alarmVector each active bit represents one alarm
 * @param alarmQuantity counter of silenced alarms
 */
void Hmi_GetSilencedAlarms(uint32_t * alarmVector, uint32_t * alarmQuantity);

/**
 * Dispatch key to other modules before handle itself.
 * VentilatorMgr receives the events from START, STOP and EMERGENCY button.
 * AlarmMgr receives SILENCE event.
 *
 * @param keyId of the dispatched event
 * @param type #LKP or #SKP
 * @param alarmLatched true if there is an alarm being displayed, otherwise is false
 * 
 * @return #TRUE if the key was dispatched to a specific module\n
 *         #FALSE any module receives the key
 */
bool Hmi_DispatchKey(KeyIdType keyId, KeyPressType type, bool alarmLatched);

/**
 * Dispatch Inspiratory Time #PARAM_TI when it changes.
 *
 * @param value New value
 */
void Hmi_DispatchInspiratoryTime(uint32_t value);

/**
 * Dispatch Tildal Volume #PARAM_VT when it changes.
 *
 * @param value New value
 */
void Hmi_DispatchTildalVolume(uint32_t value);

/**
 * Dispatch Inspiratory Pressure #PARAM_PI when it changes.
 *
 * @param value New value
 */
void Hmi_DispatchInspiratoryPressure(uint32_t value);

/**
 * Dispatch Respiratory Rate #PARAM_FR when it changes.
 *
 * @param value New value
 */
void Hmi_DispatchRespiratorioRate(uint32_t value);

/**
 * Dispatch Control Mode when it changes.
 *
 * @param type #HMI_MOTOR_CONTROL_VOLUME or #HMI_MOTOR_CONTROL_PRESSURE
 */
void Hmi_DispatchControlMode(HmiMotorControlType type);

/**
 * Dispatch an alarm minimum tidal volume limit update event.
 *
 * @param value new limit expressed in ml
 */
void Hmi_DispatchSetAlarmTidalVolMin(uint32_t value);

/**
 * Dispatch an alarm minimum tidal volume limit update event.
 *
 * @param value new limit expressed in ml
 */
void Hmi_DispatchSetAlarmTidalVolMax(uint32_t value);

void Hmi_DispatchSetAlarmPIPMax(uint32_t value);

void Hmi_DispatchSetAlarmPIPMin(uint32_t value);

void Hmi_DispatchSetAlarmPEEPMin(uint32_t value);

void Hmi_DispatchSetAlarmTIError(uint32_t value);

void Hmi_DispatchSetAlarmBPMError(uint32_t value);

void Hmi_DispatchSetAlarmPIPPEEPDif(uint32_t value);

/**
 * Getter of the IE ratio recalculated when any other parameter is set.
 *
 * @param value New value
 */
void Hmi_GetIERatio(uint32_t * value);

void Hmi_GetAlarmPIPMax(uint32_t *value);

void Hmi_GetAlarmPIPMin(uint32_t *value);

void Hmi_GetAlarmPEEPMin(uint32_t *value);

void Hmi_GetAlarmTIError(uint32_t *value);

void Hmi_GetAlarmBPMError(uint32_t *value);

void Hmi_GetAlarmPIPPEEPDif(uint32_t *value);

void Hmi_GetAlarmTidalVolMax(uint32_t *value);

void Hmi_GetAlarmTidalVolMin(uint32_t *value);

#endif // _HMI_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

