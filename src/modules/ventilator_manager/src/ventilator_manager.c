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

//*******************************************************************
//!
//!   \file       ventilator_manager.c
//!
//!   \brief      This is the ventilator manager module implementation
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       29 Apr 2020
//!
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "fsm.h"
#include "logger_api.h"
#define ARM_MATH_CM3  // Use ARM Cortex M3
#include <arm_math.h>    // Include CMSIS header
#include "adc_drv_api.h"
#include "alarm_manager_api.h"

//********************************************************************
//! @addtogroup ventilator_manager_imp
//! @{
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
#define LOG_TAG         "VentMgr"

#define START_SIG         (USER_SIG)
#define ABORT_SIG         (USER_SIG+1)
#define STOP_SIG          (USER_SIG+2)
#define MOVE_CPLT_SIG     (USER_SIG+3)
#define OVERPRESSURE_SIG  (USER_SIG+4)
#define UNDERPRESSURE_SIG (USER_SIG+5)

#define VENTILATOR_MGR_INVALID_TIMESTAMP (UINT32_MAX)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef enum ventilator_pressure_trigger_id_tag
{
   VENTILATOR_PRESSURE_TRIGGER_OVERPRESSURE,
   VENTILATOR_PRESSURE_TRIGGER_UNDERPRESSURE,
}VentilatorPressureTriggerIdType;

typedef enum ventilator_par_mode_tag
{
   VENTILATOR_PARAMS_IERATIO_SET,
   VENTILATOR_PARAMS_INSPTIME_SET,
} VentilatorParamsMode;

typedef struct vol2deg_table_tag
{
   uint32_t volumeML;
   uint32_t deg;
} VolumeToDegType;

typedef struct ventilator_mgr_fsm_evt_tag
{
   Signal sig;
   uint32_t data;
} VentilatorMgrFsmEventType;

typedef struct ventilator_fsm_tag
{
   State state__; /* the current state */
   uint32_t lastTimestamp;
   uint32_t currentDistance;
   uint32_t currentInSpeed;
   uint32_t currentExSpeed;
   uint32_t currentPauseTime;
   uint32_t currentInTime;
   uint32_t currentExTime;
   int32_t currentInPressure;
   int32_t initialPressure;
   uint32_t currentMaxInTime;
   uint32_t currentMaxExTime;
   uint32_t currentMaxTidalVol;
   uint32_t currentMinTidalVol;
   uint32_t lastInhaleTimestamp;
   int32_t lastPIP;
   int32_t lastPEEP;
   int32_t lastPplt;
   Bool     phaseCompleted;
   Bool     stopOnNextCycle;
   uint32_t startTimestamp;
} VentilatorMgrFsmType;

typedef struct ventilator_mgr_tag
{
   VentilatorMgrModeControlType controlMode;
   VentilatorMgrFsmType fsm;
   VentilatorParamsMode paramMode;

   // input parameters
   uint32_t respRateBPM;
   uint32_t tidalVolumeML;
   uint32_t ieRatio;
   uint32_t inspPressure;

   // calculated parameters
   uint32_t respPeriodMillis;
   uint32_t inspiratoryTimeMillis;
   uint32_t pltTimeMillis;
   uint32_t expirationTimeMillis;
   uint32_t distanceInDeg;
   uint32_t inhaleSpeed;
   uint32_t exhaleSpeed;

   // error conditions
   uint32_t maxTidalVolume;
   uint32_t minTidalVolume;
   uint32_t maxPIP;
   uint32_t minPIP;
   uint32_t minPEEP;
   int32_t maxBPMError;
   uint32_t minPIPPEEPDif;
   uint32_t maxTIError;

   // pressure triggers
   VentilatorMgrPressureTrigType pressureTriggers[VENTILATOR_MGR_PRESSURE_MAX_TRIGGERS];

   // pid control when using pressure-control
   arm_pid_instance_q15 pid;
}VentilatorMgrType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void ventilator_mgr_on_overpressure_trigger(VentilatorMgrPressureTrigType* trigger, int32_t value);
static void ventilator_mgr_on_underpressure_trigger(VentilatorMgrPressureTrigType* trigger, int32_t value);
static StatusType ventilator_mgr_calc_params_from_ie(void);
static StatusType ventilator_mgr_calc_params_from_tin(void);
static StatusType ventilator_mgr_calc_params(void);
static StatusType ventilator_mgr_vol2deg(uint32_t vol, uint32_t *deg);
static void ventilator_mgr_fsm_init(void);
static void ventilator_mgr_fsm_initial(VentilatorMgrFsmType *me, Event const *e);
static void ventilator_mgr_fsm_inhale(VentilatorMgrFsmType *me, Event const *e);
static void ventilator_mgr_fsm_plateau(VentilatorMgrFsmType *me, Event const *e);
static void ventilator_mgr_fsm_pause(VentilatorMgrFsmType *me, Event const *e);
static void ventilator_mgr_fsm_exhale(VentilatorMgrFsmType *me, Event const *e);

static void ventilator_mgr_do_pressure_control(VentilatorMgrFsmType *me);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

static const VolumeToDegType volToDegTable[] = {
#undef X
#define X(a,b) {a, b},
      VENTILATOR_MGR_VOL_DEG_TABLE
};

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static VentilatorMgrType ventilatorMgrData;

//********************************************************************
// Function Definitions
//********************************************************************


StatusType VentilatorMgr_Init(void)
{
   StatusType err = E_OK;

   //ventilatorMgrData.controlMode = VENTILATOR_MGR_VOLUME_CONTROL;
   ventilatorMgrData.controlMode = VENTILATOR_MGR_PRESSURE_CONTROL;

   ventilatorMgrData.respRateBPM = VENTILATOR_MGR_DEFAULT_BPM; //10;
   ventilatorMgrData.tidalVolumeML = VENTILATOR_MGR_DEFAULT_TVOLUME; //300;
   ventilatorMgrData.ieRatio = VENTILATOR_MGR_DEFAULT_IE_RATIO; //100;
   ventilatorMgrData.inspiratoryTimeMillis = 1000;
   ventilatorMgrData.paramMode = VENTILATOR_PARAMS_INSPTIME_SET;
   ventilatorMgrData.pltTimeMillis = VENTILATOR_MGR_HOLD_TIME_MILLIS;
   ventilatorMgrData.inspPressure = 200;

   ventilatorMgrData.maxPIP = VENTILATOR_MGR_MAX_PIP_THRESHOLD_MAX;
   ventilatorMgrData.minPIP = VENTILATOR_MGR_MIN_PIP_THRESHOLD_MIN;
   ventilatorMgrData.minPEEP = VENTILATOR_MGR_MIN_PEEP_DEFAULT;
   ventilatorMgrData.minTidalVolume = VENTILATOR_MGR_MIN_TIDAL_VOL_THRESHOLD_MIN;
   ventilatorMgrData.maxTidalVolume = VENTILATOR_MGR_MAX_TIDAL_VOL_THRESHOLD_MAX;
   ventilatorMgrData.maxTIError = VENTILATOR_MGR_MAX_TIERROR_THRESHOLD_MAX;
   ventilatorMgrData.maxBPMError = VENTILATOR_MGR_MAX_BPMERROR_THRESHOLD_MAX;
   ventilatorMgrData.minPIPPEEPDif = VENTILATOR_MGR_MIN_PIP_VS_PEEP_THRESHOLD_MAX;

   ventilator_mgr_calc_params();
   ventilator_mgr_fsm_init();

   //module init callout
   err = VentilatorMgr_ModuleInit();
   if (E_OK != err)
   {
      return err;
   }

   // create pressure triggers
   VentilatorMgrPressureTrigType *t;
   t = &ventilatorMgrData.pressureTriggers[VENTILATOR_PRESSURE_TRIGGER_OVERPRESSURE];
   t->id = VENTILATOR_PRESSURE_TRIGGER_OVERPRESSURE;
   t->type = VENTILATOR_MGR_PRESSURE_TRIGGER_HIGHER_THAN;
   t->threshold = VENTILATOR_MGR_MAX_PIP_THRESHOLD_MAX;
   t->callback = ventilator_mgr_on_overpressure_trigger;
   err = VentilatorMgr_SetPressureTrigger(t);
   if (E_OK != err)
   {
      return err;
   }

   t = &ventilatorMgrData.pressureTriggers[VENTILATOR_PRESSURE_TRIGGER_UNDERPRESSURE];
   t->id = VENTILATOR_PRESSURE_TRIGGER_UNDERPRESSURE;
   t->type = VENTILATOR_MGR_PRESSURE_TRIGGER_LOWER_THAN;
   t->threshold = VENTILATOR_MGR_MIN_PEEP_THRESHOLD_MIN;
   t->callback = ventilator_mgr_on_underpressure_trigger;
   err = VentilatorMgr_SetPressureTrigger(t);
   if (E_OK != err)
   {
      return err;
   }

   //init pid controller
   float32_t coef;
   coef = 0.27f;
   arm_float_to_q15(&coef, &ventilatorMgrData.pid.Kp, 1);
   coef = 0.0023f;
   arm_float_to_q15(&coef, &ventilatorMgrData.pid.Ki, 1);
   coef = 0.00f;
   arm_float_to_q15(&coef, &ventilatorMgrData.pid.Kd, 1);
   arm_pid_init_q15(&ventilatorMgrData.pid, 1);

   return err;
}

StatusType VentialtorMgr_SetControlMode(VentilatorMgrModeControlType mode)
{
   StatusType err;
   if (ventilatorMgrData.fsm.state__ != (void *) ventilator_mgr_fsm_initial)
   {
      return E_ERROR;
   }

   switch(mode)
   {
      case VENTILATOR_MGR_PRESSURE_CONTROL:
      case VENTILATOR_MGR_VOLUME_CONTROL:
         ventilatorMgrData.controlMode = mode;
         err = E_OK;
         break;
      default:
         err = E_ERROR;
         break;
   }
   return err;
}

StatusType VentilatorMgr_GetControlMode(VentilatorMgrModeControlType *mode)
{
   if (NULL == mode)
      return E_ERROR;

   *mode = ventilatorMgrData.controlMode;
   return E_OK;
}

StatusType VentilatorMgr_SetParametersWithIERatio(uint32_t bpm, uint32_t volInML, uint32_t ieRatio)
{
   StatusType err;

   err = VentilatorMgr_SetRespiratoryRate(bpm);
   if (E_ERROR == err)
      return err;

   err = VentilatorMgr_SetTidalVolume(volInML);
   if (E_ERROR == err)
      return err;

   err = VentilatorMgr_SetIERatio(ieRatio);
   if (E_ERROR == err)
      return err;

   LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "T=%lu;it=%lu;et=%lu;d=%lu;is=%lu;es=%lu",
         ventilatorMgrData.respPeriodMillis,
         ventilatorMgrData.inspiratoryTimeMillis,
         ventilatorMgrData.expirationTimeMillis,
         ventilatorMgrData.distanceInDeg,
         ventilatorMgrData.inhaleSpeed,
         ventilatorMgrData.exhaleSpeed);

   return E_OK;
}

StatusType VentilatorMgr_SetParametersWithInspTime(uint32_t bpm, uint32_t volInML, uint32_t inspTimeMillis)
{
   StatusType err;

   err = VentilatorMgr_SetRespiratoryRate(bpm);
   if (E_ERROR == err)
      return err;

   err = VentilatorMgr_SetTidalVolume(volInML);
   if (E_ERROR == err)
      return err;

   err = VentilatorMgr_SetInspiratoryTime(inspTimeMillis);
   if (E_ERROR == err)
      return err;

   LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "T=%lu;it=%lu;et=%lu;d=%lu;is=%lu;es=%lu",
         ventilatorMgrData.respPeriodMillis,
         ventilatorMgrData.inspiratoryTimeMillis,
         ventilatorMgrData.expirationTimeMillis,
         ventilatorMgrData.distanceInDeg,
         ventilatorMgrData.inhaleSpeed,
         ventilatorMgrData.exhaleSpeed);

   return E_OK;
}

StatusType VentilatorMgr_SetRespiratoryRate(uint32_t bpm)
{
   if ((VENTILATOR_MGR_BPM_MIN > bpm) || (VENTILATOR_MGR_BPM_MAX < bpm))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.respRateBPM = bpm;

   return ventilator_mgr_calc_params();
}

StatusType VentilatorMgr_GetRespiratoryRate(uint32_t* bpm)
{
   if (NULL == bpm)
      return E_ERROR;

   *bpm = ventilatorMgrData.respRateBPM;
   return E_OK;
}

StatusType VentilatorMgr_SetTidalVolume(uint32_t volInML)
{
   if ((VENTILATOR_MGR_TIDAL_VOLUME_MIN > volInML) || (VENTILATOR_MGR_TIDAL_VOLUME_MAX < volInML))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.tidalVolumeML = volInML;

   return ventilator_mgr_calc_params();
}

StatusType VentilatorMgr_GetTidalVolume(uint32_t *volInML)
{
   if (NULL == volInML)
      return E_ERROR;

   *volInML = ventilatorMgrData.tidalVolumeML;
   return E_OK;
}

StatusType VentilatorMgr_SetIERatio(uint32_t ieRatio)
{
   if ((VENTILATOR_MGR_IE_RATIO_MIN > ieRatio) || (VENTILATOR_MGR_IE_RATIO_MAX < ieRatio))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.ieRatio = ieRatio;
   ventilatorMgrData.paramMode = VENTILATOR_PARAMS_IERATIO_SET;

   return ventilator_mgr_calc_params();
}

StatusType VentilatorMgr_GetIERatio(uint32_t *ieRatio)
{
   if (NULL == ieRatio)
      return E_ERROR;

   *ieRatio = ventilatorMgrData.ieRatio;
   return E_OK;
}

StatusType VentilatorMgr_SetInspiratoryTime(uint32_t inspTimeMillis)
{
   if ((VENTILATOR_MGR_INSP_TIME_MIN_MILLIS > inspTimeMillis) || (VENTILATOR_MGR_INSP_TIME_MAX_MILLIS < inspTimeMillis))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.inspiratoryTimeMillis = inspTimeMillis;

   ventilatorMgrData.paramMode = VENTILATOR_PARAMS_INSPTIME_SET;

   return ventilator_mgr_calc_params();
}

StatusType VentilatorMgr_GetInspiratoryTime(uint32_t *inspTimeMillis)
{
   if (NULL == inspTimeMillis)
      return E_ERROR;

   *inspTimeMillis = ventilatorMgrData.inspiratoryTimeMillis;
   return E_OK;
}

StatusType VentilatorMgr_SetPlateauTime(uint32_t pltTimeMillis)
{
   if ((VENTILATOR_MGR_PLT_TIME_MIN_MILLIS > pltTimeMillis) || (VENTILATOR_MGR_PLT_TIME_MAX_MILLIS < pltTimeMillis))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.pltTimeMillis = pltTimeMillis;

   return ventilator_mgr_calc_params();
}

StatusType VentilatorMgr_GetPlateauTime(uint32_t *pltTimeMillis)
{
   if (NULL == pltTimeMillis)
      return E_ERROR;

   *pltTimeMillis = ventilatorMgrData.pltTimeMillis;
   return E_OK;
}

StatusType VentilatorMgr_SetInspiratoryPressure(uint32_t inspPressure)
{
   if ((VENTILATOR_MGR_INSP_PRESSURE_MIN > inspPressure) || (VENTILATOR_MGR_INSP_PRESSURE_MAX < inspPressure))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.inspPressure = inspPressure;

   return ventilator_mgr_calc_params();
}

StatusType VentilatorMgr_GetInspiratoryPressure(uint32_t *inspPressure)
{
   if (NULL == inspPressure)
      return E_ERROR;

   *inspPressure = ventilatorMgrData.inspPressure;
   return E_OK;
}

StatusType VentilatorMgr_SetMinTidalVolume(uint32_t minTidalVolume)
{
   if ((VENTILATOR_MGR_MIN_TIDAL_VOL_THRESHOLD_MIN > minTidalVolume) || (VENTILATOR_MGR_MIN_TIDAL_VOL_THRESHOLD_MAX < minTidalVolume))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.minTidalVolume = minTidalVolume;

   return E_OK;
}

StatusType VentilatorMgr_GetMinTidalVolume(uint32_t *minTidalVolume)
{
   if (NULL == minTidalVolume)
      return E_ERROR;

   *minTidalVolume = ventilatorMgrData.minTidalVolume;
   return E_OK;
}

StatusType VentilatorMgr_SetMaxTidalVolume(uint32_t maxTidalVolume)
{
   if ((VENTILATOR_MGR_MAX_TIDAL_VOL_THRESHOLD_MIN > maxTidalVolume) || (VENTILATOR_MGR_MAX_TIDAL_VOL_THRESHOLD_MAX < maxTidalVolume))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.maxTidalVolume = maxTidalVolume;

   return E_OK;
}

StatusType VentilatorMgr_SetMaxPIP(uint32_t maxPIP)
{
   if ((VENTILATOR_MGR_MAX_PIP_THRESHOLD_MIN > maxPIP) || (VENTILATOR_MGR_MAX_PIP_THRESHOLD_MAX < maxPIP))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.maxPIP = maxPIP;

   if (maxPIP < ventilatorMgrData.minPIP)
   {
      ventilatorMgrData.minPIP = maxPIP;
   }

   return E_OK;
}

StatusType VentilatorMgr_GetMaxPIP(uint32_t *maxPIP)
{
   if (NULL == maxPIP)
      return E_ERROR;

   *maxPIP = ventilatorMgrData.maxPIP;
   return E_OK;
}

StatusType VentilatorMgr_SetMinPIP(uint32_t minPIP)
{
   if ((VENTILATOR_MGR_MIN_PIP_THRESHOLD_MIN > minPIP) || (VENTILATOR_MGR_MIN_PIP_THRESHOLD_MAX < minPIP))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.minPIP = minPIP;

   if (minPIP > ventilatorMgrData.maxPIP)
   {
      ventilatorMgrData.maxPIP = minPIP;
   }

   return E_OK;
}

StatusType VentilatorMgr_GetMinPIP(uint32_t *minPIP)
{
   if (NULL == minPIP)
      return E_ERROR;

   *minPIP = ventilatorMgrData.minPIP;
   return E_OK;
}

StatusType VentilatorMgr_SetMaxPEEP(uint32_t maxPEEP)
{
   //not implemented
   return E_ERROR;
}

StatusType VentilatorMgr_GetMaxPEEP(uint32_t *maxPEEP)
{
   //not implemented
   return E_ERROR;
}

StatusType VentilatorMgr_SetMinPEEP(uint32_t minPEEP)
{
   if ((VENTILATOR_MGR_MIN_PEEP_THRESHOLD_MIN > minPEEP) || (VENTILATOR_MGR_MIN_PEEP_THRESHOLD_MAX < minPEEP))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.minPEEP = minPEEP;

   return E_OK;
}

StatusType VentilatorMgr_GetMinPEEP(uint32_t *minPEEP)
{
   if (NULL == minPEEP)
      return E_ERROR;

   *minPEEP = ventilatorMgrData.minPEEP;
   return E_OK;
}


StatusType VentilatorMgr_SetMaxTIError(uint32_t tiError)
{
   if ((VENTILATOR_MGR_MAX_TIERROR_THRESHOLD_MIN > tiError) || (VENTILATOR_MGR_MAX_TIERROR_THRESHOLD_MAX < tiError))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.maxTIError = tiError;

   return E_OK;
}

StatusType VentilatorMgr_GetMaxTIError(uint32_t *tiError)
{
   if (NULL == tiError)
      return E_ERROR;

   *tiError = ventilatorMgrData.maxTIError;
   return E_OK;
}

StatusType VentilatorMgr_SetMaxBPMError(uint32_t bpmError)
{
   if ((VENTILATOR_MGR_MAX_BPMERROR_THRESHOLD_MIN > bpmError) || (VENTILATOR_MGR_MAX_BPMERROR_THRESHOLD_MAX < bpmError))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.maxBPMError = bpmError;

   return E_OK;
}

StatusType VentilatorMgr_GetMaxBPMError(uint32_t *bpmError)
{
   if (NULL == bpmError)
      return E_ERROR;

   *bpmError = ventilatorMgrData.maxBPMError;
   return E_OK;
}

StatusType VentilatorMgr_SetMinPIPPEEPDif(uint32_t minPIPPEEPDif)
{
   if ((VENTILATOR_MGR_MIN_PIP_VS_PEEP_THRESHOLD_MIN > minPIPPEEPDif) || (VENTILATOR_MGR_MIN_PIP_VS_PEEP_THRESHOLD_MAX < minPIPPEEPDif))
   {
      //value out of range
      return E_ERROR;
   }
   // update internal setting and calculate new set of parameters
   ventilatorMgrData.minPIPPEEPDif = minPIPPEEPDif;

   return E_OK;
}

StatusType VentilatorMgr_GetMinPIPPEEPDif(uint32_t *minPIPPEEPDif)
{
   if (NULL == minPIPPEEPDif)
      return E_ERROR;

   *minPIPPEEPDif = ventilatorMgrData.minPIPPEEPDif;
   return E_OK;
}





StatusType VentilatorMgr_GetMaxTidalVolume(uint32_t *maxTidalVolume)
{
   if (NULL == maxTidalVolume)
      return E_ERROR;

   *maxTidalVolume = ventilatorMgrData.maxTidalVolume;
   return E_OK;
}

StatusType VentilatorMgr_Update(void)
{
   FsmDispatch(&ventilatorMgrData.fsm, (VentilatorMgrFsmEventType*)&tickEvt);

   return E_OK;
}

StatusType VentilatorMgr_Start(void)
{
   VentilatorMgrFsmEventType evt;


   evt.sig = START_SIG;

   FsmDispatch(&ventilatorMgrData.fsm, &evt);

   return E_OK;
}

StatusType VentilatorMgr_Stop(VentilatorMgrStopType type)
{
   VentilatorMgrFsmEventType evt;

   if (VENTILATOR_MGR_STOP_TYPE_EMERGENCY == type)
      evt.sig = ABORT_SIG;
   else
      evt.sig = STOP_SIG;

   FsmDispatch(&ventilatorMgrData.fsm, &evt);

   return E_OK;
}


void VentilatorMgr_onMoveComplete(void)
{
   VentilatorMgrFsmEventType evt;


   evt.sig = MOVE_CPLT_SIG;

   FsmDispatch(&ventilatorMgrData.fsm, &evt);
}


StatusType VentilatorMgr_SetPIDParameters(float32_t kp, float32_t ki, float32_t kd)
{
   arm_float_to_q15(&kp, &ventilatorMgrData.pid.Kp, 1);
   arm_float_to_q15(&ki, &ventilatorMgrData.pid.Ki, 1);
   arm_float_to_q15(&kd, &ventilatorMgrData.pid.Kd, 1);
   arm_pid_init_q15(&ventilatorMgrData.pid, 1);
   return E_OK;
}

StatusType VentilatorMgr_GetPIDParameters(float32_t *kp, float32_t *ki, float32_t *kd)
{
   if ((NULL == kp) || (NULL == ki) || (NULL == kd))
   {
      return E_ERROR;
   }

   arm_q15_to_float(&ventilatorMgrData.pid.Kp, kp, 1);
   arm_q15_to_float(&ventilatorMgrData.pid.Ki, ki, 1);
   arm_q15_to_float(&ventilatorMgrData.pid.Kd, kd, 1);

   return E_OK;
}


void ventilator_mgr_on_overpressure_trigger(VentilatorMgrPressureTrigType* trigger, int32_t value)
{
   VentilatorMgrFsmEventType evt;
   evt.sig = OVERPRESSURE_SIG;
   evt.data = value;
   FsmDispatch(&ventilatorMgrData.fsm, &evt);
}

void ventilator_mgr_on_underpressure_trigger(VentilatorMgrPressureTrigType* trigger, int32_t value)
{
   VentilatorMgrFsmEventType evt;
   evt.sig = UNDERPRESSURE_SIG;
   evt.data = value;
   FsmDispatch(&ventilatorMgrData.fsm, &evt);
}

static StatusType ventilator_mgr_calc_params(void)
{
   return (ventilatorMgrData.paramMode == VENTILATOR_PARAMS_IERATIO_SET)?
         ventilator_mgr_calc_params_from_ie():
         ventilator_mgr_calc_params_from_tin();
}
static StatusType ventilator_mgr_calc_params_from_ie(void)
{
   StatusType err = E_ERROR;
   uint32_t periodMillis;
   uint32_t tinMillis;
   uint32_t texMillis;
   uint32_t distanceInDeg, inhaleSpeed, exhaleSpeed;

   // calculate internal parameters based on ventilator settings
   periodMillis = (60 * 1e3) / ventilatorMgrData.respRateBPM;
   if (((periodMillis * 100) / (100 + ventilatorMgrData.ieRatio)) < ventilatorMgrData.pltTimeMillis)
   {
      return E_ERROR;
   }
   if (VENTILATOR_MGR_VOLUME_CONTROL == ventilatorMgrData.controlMode)
   {
      tinMillis = (periodMillis * 100) / (100 + ventilatorMgrData.ieRatio) - ventilatorMgrData.pltTimeMillis;
      texMillis = periodMillis - (tinMillis + ventilatorMgrData.pltTimeMillis);
   }
   else
   {
      tinMillis = (periodMillis * 100) / (100 + ventilatorMgrData.ieRatio);
      texMillis = periodMillis - (tinMillis);
   }

   err = ventilator_mgr_vol2deg(ventilatorMgrData.tidalVolumeML, &distanceInDeg);
   if (err != E_OK)
   {
      return E_ERROR;
   }

   inhaleSpeed = (distanceInDeg * 1e3) / tinMillis;
   exhaleSpeed = (distanceInDeg * 1e3) / texMillis;

   // update internal variables;
   ventilatorMgrData.respPeriodMillis = periodMillis;
   ventilatorMgrData.inspiratoryTimeMillis = tinMillis;
   ventilatorMgrData.expirationTimeMillis = texMillis;
   ventilatorMgrData.distanceInDeg = distanceInDeg;
   ventilatorMgrData.inhaleSpeed = inhaleSpeed;
   ventilatorMgrData.exhaleSpeed = exhaleSpeed;

   return E_OK;
}

// this functions calculates operation parameters from:
// RespiratoryRate, inspiratoryTime and TidalVolulme
static StatusType ventilator_mgr_calc_params_from_tin(void)
{
   StatusType err = E_ERROR;
   uint32_t periodMillis;
   uint32_t ieRatio;
   uint32_t texMillis;
   uint32_t distanceInDeg, inhaleSpeed, exhaleSpeed;

   // calculate internal parameters based on ventilator settings
   periodMillis = (60 * 1e3) / ventilatorMgrData.respRateBPM;
   //tinMillis = periodMillis / (1 + ventilatorMgrData.ieRatio) - VENTILATOR_MGR_HOLD_TIME_MILLIS;
   if (periodMillis < (ventilatorMgrData.inspiratoryTimeMillis + ventilatorMgrData.pltTimeMillis))
   {
      return E_ERROR;
   }

   if (VENTILATOR_MGR_VOLUME_CONTROL == ventilatorMgrData.controlMode)
   {
      texMillis = periodMillis - (ventilatorMgrData.inspiratoryTimeMillis + ventilatorMgrData.pltTimeMillis);
      ieRatio = (100 * texMillis) / (ventilatorMgrData.inspiratoryTimeMillis + ventilatorMgrData.pltTimeMillis);
   }
   else
   {
      texMillis = periodMillis - (ventilatorMgrData.inspiratoryTimeMillis);
      ieRatio = (100 * texMillis) / (ventilatorMgrData.inspiratoryTimeMillis);
   }

   err = ventilator_mgr_vol2deg(ventilatorMgrData.tidalVolumeML, &distanceInDeg);
   if (err != E_OK)
   {
      return E_ERROR;
   }

   inhaleSpeed = (distanceInDeg * 1e3) / ventilatorMgrData.inspiratoryTimeMillis;
   exhaleSpeed = (distanceInDeg * 1e3) / texMillis;

   // update internal variables;
   ventilatorMgrData.respPeriodMillis = periodMillis;
   ventilatorMgrData.expirationTimeMillis = texMillis;
   ventilatorMgrData.ieRatio = ieRatio;
   ventilatorMgrData.distanceInDeg = distanceInDeg;
   ventilatorMgrData.inhaleSpeed = inhaleSpeed;
   ventilatorMgrData.exhaleSpeed = exhaleSpeed;

   return E_OK;
}


static StatusType ventilator_mgr_vol2deg(uint32_t vol, uint32_t *deg)
{
   uint32_t i, tableSize;
   float deg_f;
   const VolumeToDegType *eLow, *eHigh;

   tableSize = sizeof(volToDegTable) / sizeof(volToDegTable[0]);

   for (i = 0; i < tableSize; i++)
   {
      eHigh = &volToDegTable[i];

      if (eHigh->volumeML >= vol)
         break;
   }

   if (i > 0)
   {
      eLow = &volToDegTable[i-1];
      deg_f = (1.0f * eLow->deg) + 1.0f * (vol - eLow->volumeML)*(eHigh->deg - eLow->deg) / (1.0f * (eHigh->volumeML - eLow->volumeML));
      *deg = (uint32_t) deg_f;
   }
   else if (i == 0)
   {
      eLow = eHigh;
      eHigh = &volToDegTable[i+1];
      deg_f = (1.0f * eLow->deg) - 1.0f * (vol - eLow->volumeML)*(eHigh->deg - eLow->deg) / (1.0f * (eHigh->volumeML - eLow->volumeML));
      *deg = (uint32_t) deg_f;
   }
   else if (i == tableSize)
   {
      eLow = &volToDegTable[i-2];
      deg_f = (1.0f * eHigh->deg) + 1.0f * (vol - eHigh->volumeML)*(eHigh->deg - eLow->deg) / (1.0f *(eHigh->volumeML - eLow->volumeML));
      *deg = (uint32_t) deg_f;
   }

   return E_OK;
}

static void ventilator_mgr_fsm_init(void)
{
   FsmCtor((Fsm*)&ventilatorMgrData.fsm, ventilator_mgr_fsm_initial);
   FsmInit((Fsm*)&ventilatorMgrData.fsm, (Event*)&entryEvt);
}

static void ventilator_mgr_fsm_initial(VentilatorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
//   VentilatorMgrFsmEventType *evt = (VentilatorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "ini", "entry");
         me->lastTimestamp = ticks;
         me->lastInhaleTimestamp = VENTILATOR_MGR_INVALID_TIMESTAMP;
         //stop any pressure measurement cycle
         VentilatorMgr_StopPressureMeasurement();
         VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_HOME, 0, 0);
         VentilatorMgr_OnStateChange(VENTILATOR_MGR_STATE_IDLE);
         break;

      case START_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "ini", "start");
         me->startTimestamp = ticks;
         FsmTran(me, ventilator_mgr_fsm_inhale);
         break;

      case EXIT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "ini", "exit");
         break;
      case TICK_SIG:
         //Logger_WriteLine("Motor", "s=%s", "ini");
      default:
         break;
   }
}

static void ventilator_mgr_fsm_inhale(VentilatorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   VentilatorMgrFsmEventType *evt = (VentilatorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
      {
         uint32_t currentBPM;
         int32_t errorBPM;

         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "inhale", "entry");
         me->lastTimestamp = ticks;
         // latch current parameters because they could change in the middle of the cycle
         me->currentDistance = ventilatorMgrData.distanceInDeg;
         me->currentInSpeed = ventilatorMgrData.inhaleSpeed;
         me->currentExSpeed = ventilatorMgrData.exhaleSpeed;
         me->currentPauseTime = ventilatorMgrData.pltTimeMillis - VENTILATOR_MGR_PLATEAU_MEAS_TIME;
         me->currentMaxInTime = (ventilatorMgrData.inspiratoryTimeMillis * (100 + ventilatorMgrData.maxTIError)) / 100;
         me->currentMaxExTime = (ventilatorMgrData.expirationTimeMillis * 110) / 100;
         me->currentInTime = ventilatorMgrData.inspiratoryTimeMillis + 50;
         me->currentExTime = ventilatorMgrData.expirationTimeMillis - 50;
         me->currentMinTidalVol = ventilatorMgrData.minTidalVolume;
         me->currentMaxTidalVol = ventilatorMgrData.maxTidalVolume;
         //me->currentInPressure = ADCDrv_GetValue(AIN_CH2,TRUE); //0; //ventilatorMgrData.inspPressure;
         me->currentInPressure = ventilatorMgrData.inspPressure;
         me->initialPressure = me->currentInPressure;
         me->phaseCompleted = FALSE;
         me->stopOnNextCycle = FALSE;

         // calculate the real respiratory rate
         if (me->lastInhaleTimestamp != VENTILATOR_MGR_INVALID_TIMESTAMP)
         {
            // calculate BPM
            currentBPM  = (60 * 10000UL) / (ticks - me->lastInhaleTimestamp);
            currentBPM += ((currentBPM % 10UL) > 5)? 10 : 0;
            currentBPM  = currentBPM / 10UL;
            // calculate relative error
            errorBPM = (100 * ((int32_t)ventilatorMgrData.respRateBPM - (int32_t)currentBPM)) / ((int32_t)ventilatorMgrData.respRateBPM);
            if ((errorBPM < (-ventilatorMgrData.maxBPMError)) ||
                  (errorBPM > ventilatorMgrData.maxBPMError))
            {
               //our current rate is out of the limits. we set an alarm and continue
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_RESPRATENOTMET, (void*)&currentBPM);
            }

            // check for airflow disconnection
            int32_t min, max, diff;
            // calculate lowest pressure
            min = (me->lastPIP > me->lastPEEP)? me->lastPEEP : me->lastPIP;
            //min = (min > me->lastPplt)? me->lastPplt : min;

            // calculate highest pressure
            max = (me->lastPIP < me->lastPEEP)? me->lastPEEP : me->lastPIP;
            //max = (max < me->lastPplt)? me->lastPplt : max;

            // calculate diff
            diff = max - min;

            if (diff < VENTILATOR_MGR_AIRFLOWDISCONN_THRESHOLD)
            {
               //all pressures are almost equal, we generate a low pressure error
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_AIRFLOWDISCONN, NULL);
            }

            if (diff < (int32_t)ventilatorMgrData.minPIPPEEPDif)
            {
               if (diff < 0)
                  diff = 0;
               //all pressures are almost equal, we generate a low pressure error
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_DRIVINGPRESSURE, &diff);
            }

            // check the air volume delivered
            int32_t deliveredVolume;
            VentilatorMgr_GetVolumeMeasurement(&deliveredVolume);
            if (deliveredVolume < me->currentMinTidalVol)
            {
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_MINTVNOTREACHED, &deliveredVolume);
            }
            else if (deliveredVolume > me->currentMaxTidalVol)
            {
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_MAXTVEXCEEDED, &deliveredVolume);
            }

            if ((ticks - me->startTimestamp) > VENTILATOR_MGR_MAINTENANCE_PERIOD_MILLIS)
            {
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_MAINTENANCEREQ, NULL);
               me->startTimestamp = ticks;
            }

         }
         me->lastInhaleTimestamp = ticks;

         //init a pressure measurement cycle
         VentilatorMgr_StartPressureMeasurement();

         VentilatorMgr_OnStateChange(VENTILATOR_MGR_STATE_INHALE);

         if (VENTILATOR_MGR_VOLUME_CONTROL == ventilatorMgrData.controlMode)
         {
            VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_COMPRESS_SPEED, me->currentDistance, me->currentInSpeed);
         }
         else
         {
            //reset pid controller
            arm_pid_reset_q15(&ventilatorMgrData.pid);

            //start compressing
            VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_COMPRESS_DRIVE, 40, 0);
         }
         break;
      }
      case ABORT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "inhale", "abort");
         FsmTran(me, ventilator_mgr_fsm_initial);
         break;

      case STOP_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "inhale", "stop");
         me->stopOnNextCycle = TRUE;
         break;

      case MOVE_CPLT_SIG:
         if (VENTILATOR_MGR_VOLUME_CONTROL == ventilatorMgrData.controlMode)
         {
            //the motor reached the final position. We store this event and we wait for the next tick
            //to see if we also met the required time
            me->phaseCompleted = TRUE;
         }
         else
         {
            //the motor reached the final position. This should never happen since we are controlling pressure
            //VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_HIGHPRESSURE, (void*)&pressureValue);
            //FsmTran(me, ventilator_mgr_fsm_exhale);
         }
         break;

      case EXIT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "inhale", "exit");
         break;

      case OVERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;

         //the pressure is above the maximum. we should set an alarm and abort this phase
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_HIGHPRESSURE, (void*)&pressureValue);
         FsmTran(me, ventilator_mgr_fsm_plateau);
         break;
      }

      case UNDERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //the pressure is below the minimum. we should set an alarm but continue cycling
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_LOWPRESSURE, (void*)&pressureValue);

         break;
      }

      case TICK_SIG:
      {
         // check door state
//         if (IOReadPinID(VENTILATOR_MGR_DOOR_INPUT_PIN))
//         {
//            //door is open, inform the error and continue cycling
//            VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_DOOROPEN, NULL);
//         }
         if (VENTILATOR_MGR_VOLUME_CONTROL == ventilatorMgrData.controlMode)
         {
            // check if we have completed this phase
            if ((me->phaseCompleted) && ((ticks - me->lastTimestamp) > me->currentInTime))
            {
               VentilatorMgrPressureMeasStatsType pressure;
               //we reach the end of the inspiratory phase; measure PIP and go to plateau phase
               if (E_OK == VentilatorMgr_GetPressureMeasurementStats(&pressure))
               {
                  VentilatorMgr_onPressureMeasurement(VENTILATOR_MGR_PRESSURE_TYPE_PIP, pressure.max, pressure.tmax);
                  if (pressure.max > (int32_t)ventilatorMgrData.maxPIP)
                     VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_PIPEXCEEDED, (void*)&pressure.max);
                  else if (pressure.max < (int32_t)ventilatorMgrData.minPIP)
                     VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_PIPNOTREACHED, (void*)&pressure.max);

                  me->lastPIP = pressure.max;
               }
               FsmTran(me, ventilator_mgr_fsm_plateau);
            }

            // check if we haven't met the time to complete this phase
            if ((!me->phaseCompleted) && ((ticks - me->lastTimestamp) > me->currentMaxInTime))
            {
               //we havent met the inspiratory time we set an error and we try to continue cycling
               VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_INSTIMEEXCEEDED, (void*)&me->currentMaxInTime);
               FsmTran(me, ventilator_mgr_fsm_plateau);
            }
         }
         else
         {
            // we nee to run the pressure-control algorithm
            ventilator_mgr_do_pressure_control(me);
            // check if we have completed this phase
            if ((ticks - me->lastTimestamp) > me->currentInTime)
            {
               VentilatorMgrPressureMeasStatsType pressure;
               //we reach the end of the inspiratory phase; measure PIP and go to plateau phase
               if (E_OK == VentilatorMgr_GetPressureMeasurementStats(&pressure))
               {
                  VentilatorMgr_onPressureMeasurement(VENTILATOR_MGR_PRESSURE_TYPE_PIP, pressure.max, pressure.tmax);

                  if (pressure.max > (int32_t)ventilatorMgrData.maxPIP)
                     VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_PIPEXCEEDED, (void*)&pressure.max);
                  else if (pressure.max < (int32_t)ventilatorMgrData.minPIP)
                     VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_PIPNOTREACHED, (void*)&pressure.max);

                  me->lastPIP = pressure.max;
               }
               VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_BRAKE, 0, 0);
               FsmTran(me, ventilator_mgr_fsm_exhale);
            }

         }
         break;
      }
      default:
         break;
   }
}

static void ventilator_mgr_do_pressure_control(VentilatorMgrFsmType *me)
{
   int32_t pressure;
   int32_t errorSignal, controlOut, newDriveLvl;
   uint32_t ticks = HAL_GetTick();

   // sample pressure signal
   if (E_OK != VentilatorMgr_GetPressureMeasurement(&pressure))
   {
      return;
   }

   //calculate error signal
   errorSignal = (me->currentInPressure - pressure) * 4;
   errorSignal = (0x4000 * errorSignal)/ me->currentInPressure;
   errorSignal = __SSAT(errorSignal,16);

   controlOut = arm_pid_q15(&ventilatorMgrData.pid, (q15_t)errorSignal);
   controlOut = (controlOut * 200) / 16384;

   LOG_PRINT_INFO(DEBUG_MOTOR_DRV, "MotorDrv", "s=%lu;e=%lu;p=%d;s=%lu;e=%d;c=%d", 1, 0, me->currentInPressure, pressure, errorSignal, controlOut);

   newDriveLvl = controlOut;
   if (newDriveLvl < 0)
      newDriveLvl = 0;
   if (newDriveLvl > 40)
      newDriveLvl = 40;

   //update motor control
   VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_UPDATE_DRIVE, 40, newDriveLvl);

   //create pressure ramp from current pressure to setpoint
   if (((ticks - me->lastTimestamp) < 350))
   {
      me->currentInPressure = ((ventilatorMgrData.inspPressure - me->initialPressure)* ((ticks - me->lastTimestamp) - 0)) / 350 + me->initialPressure;
   }
   else
   {
      me->currentInPressure = ventilatorMgrData.inspPressure;
   }
}


static void ventilator_mgr_fsm_plateau(VentilatorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   VentilatorMgrFsmEventType *evt = (VentilatorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "plt", "entry");
         me->lastTimestamp = ticks;

         //init a pressure measurement cycle
         VentilatorMgr_StartPressureMeasurement();

         VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_BRAKE, 0, 0);
         VentilatorMgr_OnStateChange(VENTILATOR_MGR_STATE_PLATEAU);
         break;

      case ABORT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "plt", "abort");
         FsmTran(me, ventilator_mgr_fsm_initial);
         break;

      case STOP_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "plt", "stop");
         me->stopOnNextCycle = TRUE;
         break;

      case EXIT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "plt", "exit");
         break;


      case OVERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //Logger_WriteLine("ADCT", "ch=%ul;v=%ld", 0, pressureValue);
         //the pressure is above the maximum. we should set an alarm
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_HIGHPRESSURE, (void*)&pressureValue);
         break;
      }

      case UNDERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //the pressure is below the minimum. we should set an alarm
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_LOWPRESSURE, (void*)&pressureValue);
         break;
      }

      case TICK_SIG:
      {
         // check door state
//         if (IOReadPinID(VENTILATOR_MGR_DOOR_INPUT_PIN))
//         {
//            //door is open, inform the error and continue cycling
//            VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_DOOROPEN, NULL);
//         }
         if ((ticks - me->lastTimestamp) > VENTILATOR_MGR_PLATEAU_MEAS_TIME)
         {
            VentilatorMgrPressureMeasStatsType pressure;
            //we reach the end of the phase phase; we measure PLT pressure and move to next phase
            if (E_OK == VentilatorMgr_GetPressureMeasurementStats(&pressure))
            {
               VentilatorMgr_onPressureMeasurement(VENTILATOR_MGR_PRESSURE_TYPE_PLT, pressure.min, pressure.tmin);
               me->lastPplt = pressure.min;
            }
            FsmTran(me, ventilator_mgr_fsm_pause);
         }
         break;
      }
      default:
         break;
   }
}

static void ventilator_mgr_fsm_pause(VentilatorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   VentilatorMgrFsmEventType *evt = (VentilatorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "pause", "entry");
         me->lastTimestamp = ticks;
         VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_BRAKE, 0, 0);
         VentilatorMgr_OnStateChange(VENTILATOR_MGR_STATE_PAUSE);
         break;

      case ABORT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "pause", "abort");
         FsmTran(me, ventilator_mgr_fsm_initial);
         break;
      case STOP_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "pause", "stop");
         me->stopOnNextCycle = TRUE;
         break;
      case EXIT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "pause", "exit");
         break;

      case OVERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //the pressure is above the maximum. we should set an alarm
         LOG_PRINT_INFO(DEBUG_ADCT, "ADCT", "ch=%ul;v=%ld", 0, pressureValue);
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_HIGHPRESSURE, (void*)&pressureValue);

         break;
      }

      case UNDERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //the pressure is below the minimum. we should set an alarm
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_LOWPRESSURE, (void*)&pressureValue);
         break;
      }

      case TICK_SIG:
         // check door state
//         if (IOReadPinID(VENTILATOR_MGR_DOOR_INPUT_PIN))
//         {
//            //door is open, inform the error and continue cycling
//            VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_DOOROPEN, NULL);
//         }
         if ((ticks - me->lastTimestamp) > (me->currentPauseTime))
         {
            //we reach the end of the phase phase
            FsmTran(me, ventilator_mgr_fsm_exhale);
         }
         break;
      default:
         break;
   }
}

static void ventilator_mgr_fsm_exhale(VentilatorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   uint32_t errorValue;
   VentilatorMgrFsmEventType *evt = (VentilatorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "exhale", "entry");
         me->lastTimestamp = ticks;
         me->phaseCompleted = FALSE;

         //init a pressure measurement cycle
         VentilatorMgr_StartPressureMeasurement();

         VentilatorMgr_OnStateChange(VENTILATOR_MGR_STATE_EXHALE);

         // we release the ambu and go to the home position
         VentilatorMgr_SetMotorState(VENTILATOR_MGR_MOTOR_RELEASE, VENTILATOR_MGR_MOTOR_HOME_DISTANCE, VENTILATOR_MGR_RELEASE_SPEED);

         break;

      case ABORT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "exhale", "abort");
         FsmTran(me, ventilator_mgr_fsm_initial);
         break;
      case STOP_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "exhale", "stop");
         me->stopOnNextCycle = TRUE;
         break;

      case MOVE_CPLT_SIG:
         // the motor reached the home position. We store this event and wait for the expiration timer to expire
         me->phaseCompleted = TRUE;
         break;

      case EXIT_SIG:
         LOG_PRINT_INFO(DEBUG_VENT_MGR, LOG_TAG, "s=%s;e=%s", "exhale", "exit");
         break;

      case OVERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //the pressure is above the maximum. we should set an alarm
         //Logger_WriteLine("ADCT", "ch=%ul;v=%ld", 0, pressureValue);
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_HIGHPRESSURE, (void*)&pressureValue);

         break;
      }

      case UNDERPRESSURE_SIG:
      {
         int32_t pressureValue = (int32_t)evt->data;
         //the pressure is below the minimum. we should set an alarm
         VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_LOWPRESSURE, (void*)&pressureValue);

         break;
      }
      case TICK_SIG:
      {
         Bool needTransition = FALSE;
         // check door state
//         if (IOReadPinID(VENTILATOR_MGR_DOOR_INPUT_PIN))
//         {
//            //door is open, inform the error and continue cycling
//            VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_DOOROPEN, NULL);
//         }
         if ((me->phaseCompleted) && ((ticks - me->lastTimestamp) > me->currentExTime))
         {
            VentilatorMgrPressureMeasStatsType pressure;

            //we reach the end of the expiration phase; we measure PEEP and continue cycling if needed
            if (E_OK == VentilatorMgr_GetPressureMeasurementStats(&pressure))
            {
               VentilatorMgr_onPressureMeasurement(VENTILATOR_MGR_PRESSURE_TYPE_PEEP, pressure.min, pressure.tmin);

               if (pressure.min < (int32_t)ventilatorMgrData.minPEEP)
                  VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_PEEPMINNOTREACHED, (void*)&pressure.min);

               me->lastPEEP = pressure.min;
            }
            needTransition = TRUE;
         }
         // check if we haven't met the time to complete this phase
         if ((!me->phaseCompleted) && ((ticks - me->lastTimestamp) > me->currentMaxExTime))
         {
            //we havent met the inspiratory time we set an error and we try to continue cycling
            VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_EXPTIMEEXCEEDED, (void *) AM_IF_MOTOR_ET_NOT_REACHED);
            errorValue = AM_IF_MOTOR_ET_NOT_REACHED;
            VentilatorMgr_OnError(VENTILATOR_MGR_ERROR_EXPTIMEEXCEEDED, (void *) &errorValue);
            needTransition = TRUE;
         }

         if (TRUE == needTransition)
         {
            if (FALSE == me->stopOnNextCycle)
            {
               FsmTran(me, ventilator_mgr_fsm_inhale);
            }
            else
            {
               FsmTran(me, ventilator_mgr_fsm_initial);
            }
         }
         break;
      }
      default:
         break;
   }
}


//********************************************************************
//
// Close the Doxygen group.
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

