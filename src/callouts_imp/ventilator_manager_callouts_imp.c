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
//!   \file       ventilator_manager_callouts_imp.c
//!
//!   \brief      This is the ventilator manager callouts implementation.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       29 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "motor_drv_api.h"
#include "rotary_enc_drv_api.h"
#include "adc_drv_api.h"
#include "logger_api.h"
#include "dflow_meter_drv_api.h"
#include "hmi_api.h"
#include "alarm_manager_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "ventilator_manager_conf.h"
#include "ventilator_manager_api.h"
#include "ventilator_manager_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define MAX_PRESSURE_DEBOUNCE_COUNTER  (40)

#define SET_ERROR_FLAG(e) do { \
                           errorFlags |= (1UL << (e)); \
                          } while(0)

#define GET_ERROR_FLAG(e) ((errorFlags & (1UL<<(e))) == (1UL<<(e)))

#define RESET_ERROR_FLAGS() do { errorFlags = 0; } while(0)

#define SET_ALARM_FLAG(e) do { \
                           alarmFlags |= (1UL << (e)); \
                          } while(0)

#define GET_ALARM_FLAG(e) ((alarmFlags & (1UL<<(e))) == (1UL<<(e)))
#define RESET_ALARM_FLAG(e) do { alarmFlags &= ~(1UL<<(e)); } while(0)
#define RESET_ALARM_FLAGS() do { alarmFlags = 0; } while(0)

#define NO_ALARM  (-1L)

#define ERRORS_TO_ALARMS_MAPPING \
X(VENTILATOR_MGR_ERROR_NOERROR         , NO_ALARM               ) \
X(VENTILATOR_MGR_ERROR_HIGHPRESSURE    , AM_OVER_PRESSURE       ) \
X(VENTILATOR_MGR_ERROR_LOWPRESSURE     , AM_UNDER_PRESSURE      ) \
X(VENTILATOR_MGR_ERROR_INSTIMEEXCEEDED , AM_INSPIRATORY_TIME_UNREACHED) \
X(VENTILATOR_MGR_ERROR_EXPTIMEEXCEEDED , AM_INTERNAL_FAILURE    ) \
X(VENTILATOR_MGR_ERROR_RESPRATENOTMET  , AM_RESP_FREQ_NOT_MET   ) \
X(VENTILATOR_MGR_ERROR_GENERALFAILURE  , AM_INTERNAL_FAILURE    ) \
X(VENTILATOR_MGR_ERROR_DOOROPEN        , AM_DOOR_OPEN           ) \
X(VENTILATOR_MGR_ERROR_AIRFLOWDISCONN  , AM_AIRFLOW_ERROR       ) \
X(VENTILATOR_MGR_ERROR_MAXTVEXCEEDED   , AM_TIDAL_VOLUME_HIGH   ) \
X(VENTILATOR_MGR_ERROR_MAXTVNOTREACHED , AM_TIDAL_VOLUME_LOW    ) \
X(VENTILATOR_MGR_ERROR_PIPEXCEEDED      ,AM_PIP_EXCEEDED        ) \
X(VENTILATOR_MGR_ERROR_PIPNOTREACHED    ,AM_PIP_NOTREACHED      ) \
X(VENTILATOR_MGR_ERROR_PEEPMINNOTREACHED,AM_UNDER_PRESSURE      ) \
X(VENTILATOR_MGR_ERROR_DRIVINGPRESSURE  ,AM_DRIVING_PRESSURE_LOW ) \
X(VENTILATOR_MGR_ERROR_MAINTENANCEREQ   ,AM_MAINTENANCE_REQUIRED )

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void CheckForClearedAlarms(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static uint32_t usedTriggers;
static ADCDrvTriggerConfType pressureTriggers[VENTILATOR_MGR_PRESSURE_MAX_TRIGGERS];
static uint32_t pressureTriggersDebounce[VENTILATOR_MGR_PRESSURE_MAX_TRIGGERS];
static uint32_t errorFlags;
static uint32_t alarmFlags;


static const int32_t error2alarm_map[] =
{
#undef X
#define X(a,b) b,
      ERRORS_TO_ALARMS_MAPPING
};

//********************************************************************
// Function Definitions
//********************************************************************
StatusType VentilatorMgr_ModuleInit(void)
{
   uint32_t i;

   usedTriggers = 0;
   errorFlags = 0;
   alarmFlags = 0;

   for(i=0; i < sizeof(pressureTriggersDebounce)/sizeof(pressureTriggersDebounce[0]); i++)
   {
      pressureTriggersDebounce[i] = 0;
   }

   return E_OK;
}

StatusType VentilatorMgr_SetMotorState(VentilatorMotorStateType state, int32_t distance, int32_t speed)
{
   StatusType err;
   switch (state)
   {
      case VENTILATOR_MGR_MOTOR_STOP:
         err = MotorDrv_Stop(MOTOR_STOP_NORMAL);
         break;
      case VENTILATOR_MGR_MOTOR_HOME:
         err = MotorDrv_GoHome();
         break;
      case VENTILATOR_MGR_MOTOR_COMPRESS_SPEED:
         err = MotorDrv_MoveDistanceAtSpeed(MOTOR_DIR_CCW, distance, speed);
         break;
      case VENTILATOR_MGR_MOTOR_RELEASE:
         err = MotorDrv_MoveDistanceAtSpeed(MOTOR_DIR_CW, distance, speed);
         break;
      case VENTILATOR_MGR_MOTOR_COMPRESS_DRIVE:
         err = MotorDrv_MoveDistanceAtDrive(MOTOR_DIR_CCW, distance, speed);
         break;
      case VENTILATOR_MGR_MOTOR_UPDATE_DRIVE:
         err = MotorDrv_ChangeDriveLevel(speed);
         break;
      case VENTILATOR_MGR_MOTOR_BRAKE:
         err = MotorDrv_Stop(MOTOR_STOP_BRAKE);
         break;
      default:
         err = E_ERROR;
   }

   return err;
}

void VentilatorMgr_OnStateChange(VentilatorStateType state)
{

   switch(state)
   {
      case VENTILATOR_MGR_STATE_IDLE:
         Hmi_UpdateVentilatorState(HMI_VENTILATOR_STATE_IDLE);
         RESET_ERROR_FLAGS();
         CheckForClearedAlarms();
         break;
      case VENTILATOR_MGR_STATE_INHALE:
         Hmi_UpdateVentilatorState(HMI_VENTILATOR_STATE_CYCLING);
         Hmi_UpdateTidalVolume(DFlowMeterDrv_GetVolume());
         DFlowMeterDrv_ResetVolume();
         CheckForClearedAlarms();
         RESET_ERROR_FLAGS();
         break;
      case VENTILATOR_MGR_STATE_PLATEAU:

         break;
      case VENTILATOR_MGR_STATE_PAUSE:

         break;
      case VENTILATOR_MGR_STATE_EXHALE:
         //Hmi_UpdateTidalVolume(DFlowMeterDrv_GetVolume());

         break;
      default:
         break;
   }
}

static void onADCTriggerEventFnt(ADCDrvTriggerConfType *trigger, int32_t value)
{
   if ((NULL != trigger) && (NULL != trigger->pUserData))
   {
      VentilatorMgrPressureTrigType *t = (VentilatorMgrPressureTrigType *)trigger->pUserData;
      if (pressureTriggersDebounce[t->id]++ > MAX_PRESSURE_DEBOUNCE_COUNTER)
      {
         (*t->callback)(t, value);
         pressureTriggersDebounce[t->id] = 0;
      }
      ADCDrv_ResetTrigger(trigger);
   }
}

StatusType VentilatorMgr_SetPressureTrigger(VentilatorMgrPressureTrigType *trigger)
{
   ADCDrvTriggerConfType *t = &pressureTriggers[usedTriggers++];

   t->channel = AIN_PRESSURE;
   t->type = (trigger->type == VENTILATOR_MGR_PRESSURE_TRIGGER_LOWER_THAN)?
               ADC_DRV_TRIGGER_TYPE_LOWER_THAN : ADC_DRV_TRIGGER_TYPE_HIGHER_THAN;
   t->threshold = trigger->threshold;
   t->triggerFnt = onADCTriggerEventFnt;
   t->pUserData = (void*) trigger;

   return ADCDrv_RegisterTrigger(t);
}

StatusType VentilatorMgr_InitPressureMeasurement(void)
{
   return E_OK;
}

inline StatusType VentilatorMgr_StartPressureMeasurement(void)
{
   return ADCDrv_ResetStats(AIN_PRESSURE);
}

StatusType VentilatorMgr_StopPressureMeasurement(void)
{
   return E_OK;
}

StatusType VentilatorMgr_GetPressureMeasurementStats(VentilatorMgrPressureMeasStatsType *value)
{
   StatusType err = E_ERROR;
   ADCDrvChStatType pressureStats;

   if (NULL == value)
   {
      return E_ERROR;
   }

   err = ADCDrv_GetStats(AIN_PRESSURE, &pressureStats, true);
   if (E_ERROR == err)
   {
      return E_ERROR;
   }

   value->min = pressureStats.min;
   value->max = pressureStats.max;
   value->avg = pressureStats.avg;
   value->value = pressureStats.value;
   value->tmax = pressureStats.tmax;
   value->tmin = pressureStats.tmin;

   //for debugging purposes
   //value->min = value->max = 100;
   return E_OK;
}

StatusType VentilatorMgr_GetPressureMeasurement(int32_t *pressure)
{
   if (pressure == NULL)
   {
      return E_ERROR;
   }

   *pressure = ADCDrv_GetValue(AIN_PRESSURE,FALSE,TRUE);

   return E_OK;
}

void VentilatorMgr_onPressureMeasurement(VentilatorMgrPressureMeasType type, int32_t value, uint32_t timestamp)
{
   switch(type)
   {
      case VENTILATOR_MGR_PRESSURE_TYPE_PEEP:
         LOG_PRINT_INFO(DEBUG_VENT_P, "VentP", "peep=%ld;tpeep=%lu", value, timestamp);
         Hmi_UpdatePEEP(value);
         break;
      case VENTILATOR_MGR_PRESSURE_TYPE_PIP:
         LOG_PRINT_INFO(DEBUG_VENT_P, "VentP", "pip=%ld;tpip=%lu", value, timestamp);
         Hmi_UpdatePIP(value);
         break;
      case VENTILATOR_MGR_PRESSURE_TYPE_PLT:
         LOG_PRINT_INFO(DEBUG_VENT_P, "VentP", "plt=%ld;tplt=%lu", value, timestamp);
         Hmi_UpdatePPlt(value);
         break;
      default:
         break;
   }
}

inline StatusType VentilatorMgr_GetVolumeMeasurement(int32_t *volume)
{
   *volume = (int32_t)DFlowMeterDrv_GetVolume();

   return E_OK;
}

inline void VentilatorMgr_OnError(VentilatorMgrErrorType error, void *pdata)
{
   int32_t *value = (int32_t*) pdata;
   Bool aflag;
   AlarmIdType alarmId;

   aflag = GET_ALARM_FLAG(error);
   if (!aflag)
   {
      LOG_PRINT_INFO(DEBUG_VENT_E, "VentE", "e=%lu;v=%ld", error, (NULL != value)? *value : 0 );
      alarmId = error2alarm_map[error];
      if (NO_ALARM != alarmId)
      {
         if (*value < 0)
            *value = 0;
         AlarmMgr_SetAlarm(alarmId, TRUE, (void*)*value);
      }
   }
   SET_ERROR_FLAG(error);
   SET_ALARM_FLAG(error);
}

static void CheckForClearedAlarms(void)
{
   uint32_t i;
   Bool eflag, aflag;
   AlarmIdType alarmId;
   for (i = 0; i < VENTILATOR_MGR_ERROR_NUM; i++)
   {
      eflag = GET_ERROR_FLAG(i);
      aflag = GET_ALARM_FLAG(i);
      if ((TRUE == aflag) && (FALSE == eflag))
      {
         alarmId = error2alarm_map[i];
         if (NO_ALARM != alarmId)
         {
            AlarmMgr_SetAlarm(alarmId, FALSE, NULL);
         }
         LOG_PRINT_INFO(DEBUG_VENT_E, "VentE", "e=%lu;c", i);

         RESET_ALARM_FLAG(i);
      }
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
