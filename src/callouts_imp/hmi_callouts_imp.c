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
//!   \file       hmi_callouts_imp.c
//!
//!   \brief      This is the HMI callouts implementation.
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       21 May 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "logger_api.h"
#include "keyboard_drv_api.h"
#include "ventilator_manager_api.h"
#include "alarm_manager_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "hmi_conf.h"
#include "hmi_api.h"
#include "hmi_callouts.h"

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
KeyIdType Hmi_ConvertKeyMapToKeyId(uint32_t keyMap)
{
   return KeyboardDrv_ConvertKeyMapToKeyId(keyMap);
}

void Hmi_GetSilencedAlarms(uint32_t * alarmVector, uint32_t * alarmQuantity)
{
   AlarmMgr_GetSilencedAlarms(alarmVector, alarmQuantity);
}

bool Hmi_DispatchKey(KeyIdType keyId, KeyPressType type, bool alarmLatched)
{
   bool handled = true;

   if (keyId == KEY_START)
   {
      VentilatorMgr_Start();
   }
   else if (alarmLatched && (KEY_PARAMETER4 == keyId))
   {
      AlarmMgr_Silence();
   }
   else if (alarmLatched && (KEY_EMERGENCY == keyId))
   {
      AlarmMgr_PauseAudio();
   }
   else
   {
      handled = false;
   }

   return handled;
}

inline void Hmi_DispatchInspiratoryTime(uint32_t value)
{
   VentilatorMgr_SetInspiratoryTime(value);
}

inline void Hmi_DispatchTildalVolume(uint32_t value)
{
   VentilatorMgr_SetTidalVolume(value);
}

inline void Hmi_DispatchInspiratoryPressure(uint32_t value)
{
   VentilatorMgr_SetInspiratoryPressure(value);
}

inline void Hmi_DispatchRespiratorioRate(uint32_t value)
{
   VentilatorMgr_SetRespiratoryRate(value);
}

inline void Hmi_DispatchControlMode(HmiMotorControlType type)
{
   VentialtorMgr_SetControlMode(type);
}



inline void Hmi_DispatchSetAlarmTidalVolMin(uint32_t value)
{
   VentilatorMgr_SetMinTidalVolume(value);
}

inline void Hmi_DispatchSetAlarmTidalVolMax(uint32_t value)
{
   VentilatorMgr_SetMaxTidalVolume(value);
}

inline void Hmi_DispatchSetAlarmPIPMax(uint32_t value)
{
   VentilatorMgr_SetMaxPIP(value);
}

inline void Hmi_DispatchSetAlarmPIPMin(uint32_t value)
{
   VentilatorMgr_SetMinPIP(value);
}

inline void Hmi_DispatchSetAlarmPEEPMin(uint32_t value)
{
   VentilatorMgr_SetMinPEEP(value);
}

inline void Hmi_DispatchSetAlarmTIError(uint32_t value)
{
   VentilatorMgr_SetMaxTIError(value);
}

inline void Hmi_DispatchSetAlarmBPMError(uint32_t value)
{
   VentilatorMgr_SetMaxBPMError(value);
}

inline void Hmi_DispatchSetAlarmPIPPEEPDif(uint32_t value)
{
   VentilatorMgr_SetMinPIPPEEPDif(value);
}




inline void Hmi_GetIERatio(uint32_t * value)
{
   VentilatorMgr_GetIERatio(value);
}

inline void Hmi_GetAlarmPIPMax(uint32_t *value)
{
   VentilatorMgr_GetMaxPIP(value);
}

inline void Hmi_GetAlarmPIPMin(uint32_t *value)
{
   VentilatorMgr_GetMinPIP(value);
}

inline void Hmi_GetAlarmPEEPMin(uint32_t *value)
{
   VentilatorMgr_GetMinPEEP(value);
}

inline void Hmi_GetAlarmTIError(uint32_t *value)
{
   VentilatorMgr_GetMaxTIError(value);
}

inline void Hmi_GetAlarmBPMError(uint32_t *value)
{
   VentilatorMgr_GetMaxBPMError(value);
}

inline void Hmi_GetAlarmPIPPEEPDif(uint32_t *value)
{
   VentilatorMgr_GetMinPIPPEEPDif(value);
}

inline void Hmi_GetAlarmTidalVolMax(uint32_t *value)
{
   VentilatorMgr_GetMaxTidalVolume(value);
}

inline void Hmi_GetAlarmTidalVolMin(uint32_t *value)
{
   VentilatorMgr_GetMinTidalVolume(value);
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
