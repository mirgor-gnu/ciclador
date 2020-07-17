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
//!   \file       hmi.c
//!
//!   \brief      This is the HMI module implementation
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "board_hw_io.h"
#include "fsm.h"
#include "logger_api.h"
#include "display_drv_api.h"
#include "keyboard_drv_api.h"
#include "ventilator_manager_api.h"
#include "alarm_manager_api.h"

//********************************************************************
//! @addtogroup hmi_imp
//! @{
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
#define LOG_TAG               "HMI"

#define SIG_NEW_KEY           (USER_SIG)
#define SIG_UPDATE_SENSOR     (USER_SIG+1)
#define SIG_SET_ALARM         (USER_SIG+2)
#define SIG_SILENCED_ALARM    (USER_SIG+3)
#define SIG_VENTILATOR_STATE  (USER_SIG+4)
#define SIG_DOUBLE_CHECK_STOP (USER_SIG+5)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
enum {
   HMI_ALARM_GO_TO_MENU,
   HMI_ALARM_GO_TO_NEXT_ALARM,
   HMI_ERROR,
};

#undef X
 #define X(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) a,
 typedef enum hmi_param_id_tag {
    HMI_PARAMS_COMMON_CFG
    HMI_SENSORS_CFG
    HMI_PARAMS_CTRL_VOLUME_CFG
    HMI_PARAMS_CTRL_PRESSURE_CFG
    HMI_SECOND_SCREEN_CFG
    HMI_PARAMS_MAX,
    HMI_PARAM_NONE,
 } HmiParamIdType;
 typedef enum hmi_adjust_alarm_id_tag {
   HMI_ADJUST_ALARM_SCREEN_1_CFG
   HMI_ADJUST_ALARM_SCREEN_2_CFG
   HMI_ADJUST_ALARM_SCREEN_3_CFG
   HMI_ADJUST_ALARM_MAX,
 } HmiAdjustAlarmIdType;

#undef X
 #define X(a,b,c,d) a,
 typedef enum hmi_setting_id_tag {
   HMI_SETTING_CFG
   HMI_SETTING_MAX,
 } HmiSettingIdType;
 typedef enum hmi_control_mode_id_tag {
   HMI_CONTROL_MODE_CFG
   HMI_CONTROL_MODE_MAX,
 } HmiControlModeIdType;
 typedef enum hmi_confirm_stop_id_tag {
   HMI_CONFIRM_STOP_CFG
   HMI_CONFIRM_STOP_MAX,
 } HmiConfirmStopIdType;

#undef X
 #define X(a,b,c,d,e,f,g,h,i) a,
 typedef enum hmi_error_id_tag {
   HMI_ERROR_CFG
   HMI_ERROR_MAX,
   HMI_ERROR_NONE,
 } HmiErrorIdType;

typedef struct hmi_cfg_element_tag
{
   HmiParamIdType       id;
   uint32_t             y;
   char *               text;
   uint32_t             textX;
   bool                 valueEnable;
   int32_t              value;
   uint32_t             valueX;
   HmiValueFormatType   valueFormat;
   uint32_t             valueMin;
   uint32_t             valueMax;
   uint32_t             valueStep;
   uint32_t             cursorEnable;
   uint32_t             cursorX;
   bool                 unitEnable;
   char *               unitText;
   uint32_t             unitX;
} HmiElementType;

typedef struct hmi_cfg_messages_tag
{
   AlarmIdType          id;
   HmiMessageTypeId     type;
   char *               line1;
   char *               line2;
   char *               line3;
   bool                 valueEnable;
   uint32_t             value;
   uint32_t             valueY;
   uint32_t             valueX;
   HmiValueFormatType   format;
} HmiMessageType;

typedef struct hmi_cfg_silenced_alarms_tag
{
   uint32_t    vector;
   uint32_t    counter;
} HmiSilencedAlarmType;

typedef struct hmi_fsm_evt_tag
{
   Signal                  sig;
   KeyIdType               keyId;
   KeyPressType            keyType;
   HmiParamIdType          sensorId;
   AlarmIdType             alarmId;
   bool                    alarmStatus;
   HmiAlarmQueueStatusType alarmQueue;
   uint32_t                alarmErrorValue;
   HmiVentilatorStateType  ventilatorState;
} HmiFsmEventType;

typedef struct hmi_fsm_tag
{
   State             state__; /* the current state */
   State             returnState;
   volatile Bool     isInitialized;
} HmiFsmType;

typedef struct hmi_tag
{
   HmiFsmType              hmiFsm;
   HmiVentilatorStateType  hmiVentilatorState;
   HmiMotorControlType     hmiMotorControl;
   HmiElementType *        hmiParams;              // TODO: review this
   HmiParamIdType          hmiSettingParamId;
   uint32_t                hmiSettingLastValue;
   AlarmIdType             hmiCurrentAlarm;
   HmiErrorIdType          hmiCurrentError;
} HmiType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void hmi_fsm_init(void);
static void hmi_fsm_main_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_second_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_edit_parameter_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_control_mode_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_adjust_alarms_1_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_adjust_alarms_2_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_adjust_alarms_3_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_edit_alarm_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_silenced_alarms_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_message_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_latched_alarm_screen(HmiFsmType *me, Event const *e);
static void hmi_fsm_confirm_stop_screen(HmiFsmType *me, Event const *e);

static void hmi_set_screen_main(void);
static void hmi_set_screen_second(void);
static void hmi_set_screen_settings(HmiParamIdType id);
static void hmi_set_screen_control_mode(void);
static void hmi_set_screen_adjust_alarm_1(void);
static void hmi_set_screen_adjust_alarm_2(void);
static void hmi_set_screen_adjust_alarm_3(void);
static void hmi_set_screen_edit_alarm(HmiParamIdType id);
static void hmi_set_screen_message(HmiMessageTypeId type, AlarmIdType id, uint32_t next);
static void hmi_set_screen_confirm_stop(void);

static void hmi_write_element(HmiElementType * element);
static void hmi_write_element_value_with_cursor(HmiElementType * element, uint8_t x, uint8_t y);
static void hmi_write_element_value(int32_t value, HmiValueFormatType format, uint8_t x, uint8_t y);
static void hmi_write_control_mode_selector(void);

static uint32_t hmi_get_next_alarm(HmiSilencedAlarmType * alarms);

static void hmi_update_alarm_values(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************
#undef X
 #define X(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p},
 HmiElementType HmiParams[] = {
    HMI_PARAMS_COMMON_CFG
    HMI_SENSORS_CFG
    HMI_PARAMS_CTRL_VOLUME_CFG
    HMI_PARAMS_CTRL_PRESSURE_CFG
    HMI_SECOND_SCREEN_CFG
 };
 HmiElementType HmiAdjustAlarm[] = {
    HMI_ADJUST_ALARM_SCREEN_1_CFG
    HMI_ADJUST_ALARM_SCREEN_2_CFG
    HMI_ADJUST_ALARM_SCREEN_3_CFG
 };

#undef X
 #define X(a,b,c,d) {a,b,c,d},
 HmiElementType HmiSettings[] = {
    HMI_SETTING_CFG
 };
 HmiElementType HmiControlMode[] = {
    HMI_CONTROL_MODE_CFG
 };
 HmiElementType HmiConfirmStop[] = {
    HMI_CONFIRM_STOP_CFG
 };

#undef X
 #define X(a,b,c,d,e,f,g,h,i) {a,b,c,d,e,f,0,g,h,i},
 HmiMessageType HmiAlarm[] = {
    HMI_ALARM_CFG
 };
 HmiMessageType HmiError[] = {
    HMI_ERROR_CFG
 };

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static HmiType hmiData = {0};

//********************************************************************
// Function Definitions
//********************************************************************
StatusType Hmi_Init(void) 
{
   StatusType err = E_OK;

   hmiData.hmiVentilatorState = HMI_VENTILATOR_STATE_IDLE;
   hmiData.hmiParams = HmiParams;
   hmiData.hmiSettingParamId = HMI_PARAM_NONE;
   hmiData.hmiCurrentAlarm = AM_ALARM_NONE;
   hmiData.hmiCurrentError = HMI_ERROR_NONE;

   err = VentilatorMgr_GetControlMode((VentilatorMgrModeControlType *) &hmiData.hmiMotorControl);
   if (E_OK != err)
   {
      return err;
   }

   err = VentilatorMgr_GetInspiratoryTime((uint32_t *) &HmiParams[PARAM_TI].value);
   if (E_OK != err)
   {
      return err;
   }
   
   err = VentilatorMgr_GetTidalVolume((uint32_t *) &HmiParams[PARAM_VT].value);
   if (E_OK != err)
   {
      return err;
   }
   
   err = VentilatorMgr_GetInspiratoryPressure((uint32_t *) &HmiParams[PARAM_PI].value);
   if (E_OK != err)
   {
      return err;
   }
   
   err = VentilatorMgr_GetRespiratoryRate((uint32_t *) &HmiParams[PARAM_FR].value);
   if (E_OK != err)
   {
      return err;
   }
   
   err = VentilatorMgr_GetIERatio((uint32_t *) &HmiParams[SENSOR_IE].value);
   if (E_OK != err)
   {
      return err;
   }

   hmi_fsm_init();

   return err;
}

StatusType Hmi_SetAlarm(AlarmIdType alarmId, Bool status, uint32_t errorValue, HmiAlarmQueueStatusType queue)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

   HmiFsmEventType evt;
   evt.sig = SIG_SET_ALARM;

   if (AM_ALARM_NONE == hmiData.hmiCurrentAlarm)
   {
      if (TRUE == status)
      {
         hmiData.hmiCurrentAlarm = alarmId;
         HmiAlarm[alarmId].value = errorValue;
         FsmDispatch(&hmiData.hmiFsm, &evt);
      }
   }
   else
   {
      evt.alarmId = alarmId;
      evt.alarmStatus = status;
      evt.alarmErrorValue = errorValue;
      evt.alarmQueue = queue;
      FsmDispatch(&hmiData.hmiFsm, &evt);
   }

   return E_OK;
}

StatusType Hmi_SetSilencedAlarms(uint32_t counter)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

   HmiParams[SS_SEE_ALARMS].value = counter;
   HmiFsmEventType evt;
   evt.sig = SIG_SILENCED_ALARM;
   FsmDispatch(&hmiData.hmiFsm, &evt);
   
   return E_OK;
}

StatusType Hmi_UpdatePEEP(uint32_t peep)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

   HmiParams[SENSOR_PEEP].value = peep;
   HmiFsmEventType evt;
   evt.sig = SIG_UPDATE_SENSOR;
   evt.sensorId = SENSOR_PEEP;
   FsmDispatch(&hmiData.hmiFsm, &evt);
   return E_OK;
}

StatusType Hmi_UpdatePIP(uint32_t pip)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

   HmiParams[SENSOR_PIP].value = pip;
   HmiFsmEventType evt;
   evt.sig = SIG_UPDATE_SENSOR;
   evt.sensorId = SENSOR_PIP;
   FsmDispatch(&hmiData.hmiFsm, &evt);

   return E_OK;
}

StatusType Hmi_UpdatePPlt(uint32_t pplt)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

//   HmiParams[SENSOR_PPLT].value = pplt;
//   HmiFsmEventType evt;
//   evt.sig = SIG_UPDATE_SENSOR;
//   evt.sensorId = SENSOR_PPLT;
//   FsmDispatch(&hmiData.hmiFsm, &evt);

   return E_OK;
}

StatusType Hmi_UpdateTidalVolume(uint32_t tidalVol)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

   HmiParams[SENSOR_VT].value = tidalVol;
   HmiFsmEventType evt;
   evt.sig = SIG_UPDATE_SENSOR;
   evt.sensorId = SENSOR_VT;
   FsmDispatch(&hmiData.hmiFsm, &evt);

   return E_OK;
}

StatusType Hmi_UpdateVentilatorState(HmiVentilatorStateType newState)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;

   hmiData.hmiVentilatorState = newState;
   if (HMI_VENTILATOR_STATE_CYCLING == newState)
   {
      IOWritePinID(IO_LED_RUN_EN, IO_ON);
   }
   else
   {
      IOWritePinID(IO_LED_RUN_EN, IO_OFF);
   }

   HmiFsmEventType evt;
   evt.sig = SIG_VENTILATOR_STATE;
   evt.ventilatorState = newState;
   FsmDispatch(&hmiData.hmiFsm, &evt);

   return E_OK;
}

StatusType Hmi_NewKeyMap(uint32_t keyMap, KeyPressType type)
{
   if (FALSE == hmiData.hmiFsm.isInitialized)
      return E_ERROR;
   
   KeyIdType keyId = NO_KEY;
   
   keyId = Hmi_ConvertKeyMapToKeyId(keyMap);
   LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "key=%d", keyId);

   if (false == Hmi_DispatchKey(keyId, type, ((void *) hmi_fsm_latched_alarm_screen == hmiData.hmiFsm.state__)))
   {
      HmiFsmEventType evt;

      if ((KEY_STOP == keyId) && (HMI_VENTILATOR_STATE_CYCLING == hmiData.hmiVentilatorState))
      {
         evt.sig = SIG_DOUBLE_CHECK_STOP;
      }
      else
      {
         evt.sig = SIG_NEW_KEY;
      }
      evt.keyId = keyId;
      evt.keyType = type;
      FsmDispatch(&hmiData.hmiFsm, &evt);
   }

   return E_OK;  
}

//********************************************************************
// Finite State Machine Functions
//********************************************************************
static void hmi_fsm_init(void)
{
   FsmCtor((Fsm*)&hmiData.hmiFsm, hmi_fsm_main_screen);
   FsmInit((Fsm*)&hmiData.hmiFsm, (Event*)&entryEvt);
   hmiData.hmiFsm.isInitialized = TRUE;
   hmiData.hmiFsm.returnState = (void *) hmi_fsm_main_screen;
}

static void hmi_fsm_main_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "main", "entry");
         hmi_set_screen_main();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "main", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER1:
               hmiData.hmiSettingParamId = PARAM_TI;
               FsmTran(me, hmi_fsm_edit_parameter_screen);
               break;
            case KEY_PARAMETER2:
               hmiData.hmiSettingParamId = (HMI_MOTOR_CONTROL_VOLUME == hmiData.hmiMotorControl) ? PARAM_VT : PARAM_PI;
               FsmTran(me, hmi_fsm_edit_parameter_screen);
               break;
            case KEY_PARAMETER3:
               hmiData.hmiSettingParamId = PARAM_FR;
               FsmTran(me, hmi_fsm_edit_parameter_screen);
               break;
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_second_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_UPDATE_SENSOR:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "main", "sensor");
         hmi_write_element_value(HmiParams[evt->sensorId].value,
                                 HmiParams[evt->sensorId].valueFormat,
                                 HmiParams[evt->sensorId].valueX,
                                 HmiParams[evt->sensorId].y);
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "main", "set_alarm");
         hmiData.hmiFsm.returnState = me->state__;
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "main", "exit");
         break;
      default:
         break;
   }
}

static void hmi_fsm_second_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sec", "entry");
         hmi_set_screen_second();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sec", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER1:
               FsmTran(me, hmi_fsm_adjust_alarms_1_screen);
               break;
            case KEY_PARAMETER2:
               FsmTran(me, hmi_fsm_silenced_alarms_screen);
               break;
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_main_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sec", "set_alarm");
         hmiData.hmiFsm.returnState = me->state__;
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_SILENCED_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sec", "new-sil-alarm");
         hmi_write_element_value(HmiParams[SS_SEE_ALARMS].value,
                                 HmiParams[SS_SEE_ALARMS].valueFormat,
                                 HmiParams[SS_SEE_ALARMS].valueX,
                                 HmiParams[SS_SEE_ALARMS].y);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sec", "exit");
         break;
      default:
         break;
   }
}

static void hmi_fsm_edit_parameter_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;
   HmiParamIdType id = hmiData.hmiSettingParamId;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-param", "entry");
         if (hmiData.hmiFsm.returnState == me->state__)
         {
            hmi_set_screen_main();
         }
         hmi_set_screen_settings(id);
         hmiData.hmiSettingLastValue = hmiData.hmiParams[id].value;
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-param", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER3:
               if (HmiParams[id].value != hmiData.hmiSettingLastValue)
               {
                  switch (id)
                  {
                     case PARAM_TI:
                        Hmi_DispatchInspiratoryTime(HmiParams[PARAM_TI].value);
                        break;
                     case PARAM_VT:
                        Hmi_DispatchTildalVolume(HmiParams[PARAM_VT].value);
                        break;
                     case PARAM_PI:
                        Hmi_DispatchInspiratoryPressure(HmiParams[PARAM_PI].value);
                        break;
                     case PARAM_FR:
                        Hmi_DispatchRespiratorioRate(HmiParams[PARAM_FR].value);
                        break;
                     default:
                        break;
                  }
                  Hmi_GetIERatio((uint32_t *) &HmiParams[SENSOR_IE].value);
               }
               FsmTran(me, hmi_fsm_main_screen);
               break;
            case KEY_PARAMETER4:
               hmiData.hmiParams[id].value = hmiData.hmiSettingLastValue;
               FsmTran(me, hmi_fsm_main_screen);
               break;
            case KEY_UP:
               if (HmiParams[id].value < HmiParams[id].valueMax)
               {
                  HmiParams[id].value += HmiParams[id].valueStep;
                  hmi_write_element_value_with_cursor(&HmiParams[id], HmiParams[id].valueX, LINE1);
               }
               break;
            case KEY_DOWN:
               if (HmiParams[id].value > HmiParams[id].valueMin)
               {
                  HmiParams[id].value -= HmiParams[id].valueStep;
                  hmi_write_element_value_with_cursor(&HmiParams[id], HmiParams[id].valueX, LINE1);
               }
               break;
            default:
               break;
         }
         break;
      case SIG_UPDATE_SENSOR:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-param", "sensor");
         hmi_write_element_value(   HmiParams[evt->sensorId].value,
                                    HmiParams[evt->sensorId].valueFormat,
                                    HmiParams[evt->sensorId].valueX,
                                    HmiParams[evt->sensorId].y);
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-param", "set_alarm");
         if (HmiParams[id].value != hmiData.hmiSettingLastValue)
         {
            hmiData.hmiParams[id].value = hmiData.hmiSettingLastValue;
         }
         hmiData.hmiFsm.returnState = me->state__;
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         if (HmiParams[id].value != hmiData.hmiSettingLastValue)
         {
            hmiData.hmiParams[id].value = hmiData.hmiSettingLastValue;
         }
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-param", "exit");
         DisplayDrv_CursorOff();
         break;
      default:
         break;
   }
}

static void hmi_fsm_control_mode_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ctrl", "entry");
         hmi_set_screen_control_mode();
         hmi_write_control_mode_selector();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ctrl", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER2:
               hmiData.hmiMotorControl = HMI_MOTOR_CONTROL_VOLUME;
               FsmTran(me, hmi_fsm_second_screen);
               break;
            case KEY_PARAMETER3:
               hmiData.hmiMotorControl = HMI_MOTOR_CONTROL_PRESSURE;
               FsmTran(me, hmi_fsm_second_screen);
               break;
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_second_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ctrl", "set_alarm");
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_VENTILATOR_STATE:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ctrl", "vent_sts");
         if (HMI_VENTILATOR_STATE_CYCLING == evt->ventilatorState)
         {
            hmiData.hmiCurrentError = ERROR_STOP_BEFORE_CHANGE_MODE;
            FsmTran(me, hmi_fsm_message_screen);
         }
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ctrl", "exit");
         Hmi_DispatchControlMode(hmiData.hmiMotorControl);
         break;
      default:
         break;
   }
}

static void hmi_fsm_adjust_alarms_1_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas1", "entry");
         hmi_update_alarm_values();
         hmi_set_screen_adjust_alarm_1();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas1", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER1:
               hmiData.hmiSettingParamId = AAS1_PEEP_MIN;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER2:
               hmiData.hmiSettingParamId = AAS1_PIP_MIN;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER3:
               hmiData.hmiSettingParamId = AAS1_PIP_MAX;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_adjust_alarms_2_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas1", "set_alarm");
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas1", "exit");
         hmiData.hmiFsm.returnState = me->state__;
         break;
      default:
         break;
   }
}

static void hmi_fsm_adjust_alarms_2_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas2", "entry");
         hmi_set_screen_adjust_alarm_2();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas2", "new_key");

         switch (evt->keyId)
         {
            case KEY_PARAMETER1:
               hmiData.hmiSettingParamId = AAS2_PIP_PEEP;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER2:
               hmiData.hmiSettingParamId = AAS2_TI_MAX;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER3:
               hmiData.hmiSettingParamId = AAS2_FR_MAX;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_adjust_alarms_3_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas2", "set_alarm");
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas2", "exit");
         hmiData.hmiFsm.returnState = me->state__;
         break;
      default:
         break;
   }
}

static void hmi_fsm_adjust_alarms_3_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas3", "entry");
         hmi_set_screen_adjust_alarm_3();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas3", "new_key");

         switch (evt->keyId)
         {
            case KEY_PARAMETER1:
               hmiData.hmiSettingParamId = AAS3_VT_MIN;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER2:
               hmiData.hmiSettingParamId = AAS3_VT_MAX;
               FsmTran(me, hmi_fsm_edit_alarm_screen);
               break;
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_second_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas3", "set_alarm");
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "aas3", "exit");
         hmiData.hmiFsm.returnState = me->state__;
         break;
      default:
         break;
   }
}

static void hmi_fsm_edit_alarm_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;
   HmiParamIdType id = hmiData.hmiSettingParamId;
   static void * returnState = NULL;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-alarm", "entry");
         hmi_set_screen_edit_alarm(id);
         hmiData.hmiSettingLastValue = HmiAdjustAlarm[id].value;
         if (hmiData.hmiFsm.returnState != (void *) hmi_fsm_edit_alarm_screen)
         {
            returnState = hmiData.hmiFsm.returnState;
         }
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-alarm", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER3:
               if (HmiAdjustAlarm[id].value != hmiData.hmiSettingLastValue)
               {
                  switch (id)
                  {
                     case AAS1_PEEP_MIN:
                        Hmi_DispatchSetAlarmPEEPMin(HmiAdjustAlarm[id].value);
                        break;
                     case AAS1_PIP_MIN:
                        Hmi_DispatchSetAlarmPIPMin(HmiAdjustAlarm[id].value);
                        break;
                     case AAS1_PIP_MAX:
                        Hmi_DispatchSetAlarmPIPMax(HmiAdjustAlarm[id].value);
                        break;
                     case AAS2_PIP_PEEP:
                        Hmi_DispatchSetAlarmPIPPEEPDif(HmiAdjustAlarm[id].value);
                        break;
                     case AAS2_TI_MAX:
                        Hmi_DispatchSetAlarmTIError(HmiAdjustAlarm[id].value);
                        break;
                     case AAS2_FR_MAX:
                        Hmi_DispatchSetAlarmBPMError(HmiAdjustAlarm[id].value);
                        break;
                     case AAS3_VT_MIN:
                        Hmi_DispatchSetAlarmTidalVolMin(HmiAdjustAlarm[id].value);
                        break;
                     case AAS3_VT_MAX:
                        Hmi_DispatchSetAlarmTidalVolMax(HmiAdjustAlarm[id].value);
                        break;
                     default:
                        break;
                  }
               }
               FsmTran(me, returnState);
               break;
            case KEY_PARAMETER4:
               HmiAdjustAlarm[id].value = hmiData.hmiSettingLastValue;
               FsmTran(me, returnState);
               break;
            case KEY_UP:
               if (HmiAdjustAlarm[id].value < HmiAdjustAlarm[id].valueMax)
               {
                  HmiAdjustAlarm[id].value += HmiAdjustAlarm[id].valueStep;
                  hmi_write_element_value_with_cursor(&HmiAdjustAlarm[id], HmiAdjustAlarm[id].valueX, LINE1);
               }
               break;
            case KEY_DOWN:
               if (HmiAdjustAlarm[id].value > HmiAdjustAlarm[id].valueMin)
               {
                  HmiAdjustAlarm[id].value -= HmiAdjustAlarm[id].valueStep;
                  hmi_write_element_value_with_cursor(&HmiAdjustAlarm[id], HmiAdjustAlarm[id].valueX, LINE1);
               }
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-alarm", "set_alarm");
         if (HmiAdjustAlarm[id].value != hmiData.hmiSettingLastValue)
         {
            HmiAdjustAlarm[id].value = hmiData.hmiSettingLastValue;
         }
         hmiData.hmiFsm.returnState = me->state__;
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         if (HmiAdjustAlarm[id].value != hmiData.hmiSettingLastValue)
         {
            HmiAdjustAlarm[id].value = hmiData.hmiSettingLastValue;
         }
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "ed-alarm", "exit");
         DisplayDrv_CursorOff();
         break;
      default:
         break;
   }
}

static void hmi_fsm_silenced_alarms_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;
   static HmiSilencedAlarmType silencedAlarms;
   uint32_t next;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sil-alarm", "entry");
         silencedAlarms.vector = 0;
         Hmi_GetSilencedAlarms(&silencedAlarms.vector, &silencedAlarms.counter);
         if (0 == silencedAlarms.counter)
         {
            FsmTran(me, hmi_fsm_second_screen);
         }
         else
         {
            next = (1 == silencedAlarms.counter) ? HMI_ALARM_GO_TO_MENU : HMI_ALARM_GO_TO_NEXT_ALARM;
            hmi_set_screen_message(HMI_MSG_ALARM, hmi_get_next_alarm(&silencedAlarms), next);
         }
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sil-alarm", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER4:
               if (0 == silencedAlarms.counter)
               {
                  FsmTran(me, hmi_fsm_second_screen);
               }
               else
               {
                  next = (1 == silencedAlarms.counter) ? HMI_ALARM_GO_TO_MENU : HMI_ALARM_GO_TO_NEXT_ALARM;
                  hmi_set_screen_message(HMI_MSG_ALARM, hmi_get_next_alarm(&silencedAlarms), next);
               }
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sil-alarm", "set_alarm");
         hmiData.hmiFsm.returnState = (void *) hmi_fsm_second_screen;
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "sil-alarm", "exit");
         break;
      default:
         break;
   }
}

static void hmi_fsm_message_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "msg", "entry");
         hmi_set_screen_message(HMI_MSG_ERROR, hmiData.hmiCurrentError, HMI_ERROR);
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "msg", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER4:
               FsmTran(me, hmi_fsm_second_screen);
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "msg", "set_alarm");
         FsmTran(me, hmi_fsm_latched_alarm_screen);
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "msg", "exit");
         hmiData.hmiCurrentError = HMI_ERROR_NONE;
         break;
      default:
         break;
   }
}

static void hmi_fsm_latched_alarm_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;
   AlarmIdType id = hmiData.hmiCurrentAlarm;
   static HmiAlarmQueueStatusType queue = HMI_ALARM_SCREEN_EMPTY_QUEUE;
   uint32_t line4;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "alarm", "entry");
         queue = HMI_ALARM_SCREEN_EMPTY_QUEUE;
         line4 = (HMI_ALARM_SCREEN_EMPTY_QUEUE == queue) ? HMI_ALARM_GO_TO_MENU : HMI_ALARM_GO_TO_NEXT_ALARM;
         hmi_set_screen_message(HMI_MSG_ALARM, id, line4);
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "alarm", "set_alarm");
         if (true == evt->alarmStatus)
         {
            if ((hmiData.hmiCurrentAlarm != evt->alarmId) || (queue != evt->alarmQueue) || (HmiAlarm[evt->alarmId].value != evt->alarmErrorValue))
            {
               HmiAlarm[evt->alarmId].value = evt->alarmErrorValue;
               queue = evt->alarmQueue;
               line4 = (HMI_ALARM_SCREEN_EMPTY_QUEUE == queue) ? HMI_ALARM_GO_TO_MENU : HMI_ALARM_GO_TO_NEXT_ALARM;
               hmiData.hmiCurrentAlarm = evt->alarmId;
               hmi_set_screen_message(HMI_MSG_ALARM, hmiData.hmiCurrentAlarm, line4);
            }
         } 
         else if (false == evt->alarmStatus)
         {
            hmiData.hmiCurrentAlarm = AM_ALARM_NONE;
            FsmTran(me, (void*) me->returnState);
         }
         break;
      case SIG_DOUBLE_CHECK_STOP:
         FsmTran(me, hmi_fsm_confirm_stop_screen);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "alarm", "exit");
         break;
      default:
         break;
   }
}

static void hmi_fsm_confirm_stop_screen(HmiFsmType *me, Event const *e)
{
   HmiFsmEventType *evt = (HmiFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "conf-stop", "entry");
         hmi_set_screen_confirm_stop();
         break;
      case SIG_NEW_KEY:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "conf-stop", "new_key");
         switch (evt->keyId)
         {
            case KEY_PARAMETER3:
               VentilatorMgr_Stop(VENTILATOR_MGR_STOP_TYPE_NORMAL);
            case KEY_PARAMETER4:
               if (AM_ALARM_NONE == hmiData.hmiCurrentAlarm)
               {
                  FsmTran(me, hmi_fsm_main_screen);
               }
               else
               {
                  FsmTran(me, hmi_fsm_latched_alarm_screen);
               }
               break;
            default:
               break;
         }
         break;
      case SIG_SET_ALARM:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "conf-stop", "set_alarm");
         hmiData.hmiCurrentAlarm = (evt->alarmStatus) ? evt->alarmId : AM_ALARM_NONE;
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_HMI, LOG_TAG, "s=%s;e=%s", "conf-stop", "exit");
         break;
      default:
         break;
   }
}

//********************************************************************
// Private Functions
//********************************************************************
static void hmi_set_screen_main(void)
{
   int i = 0;
   for (i = 0; i < 7; i++)
   {
      hmi_write_element(&HmiParams[i]);
   }

   if (HMI_MOTOR_CONTROL_PRESSURE == hmiData.hmiMotorControl)
   {
      hmi_write_element(&HmiParams[PARAM_PI]);
   }
   else if (HMI_MOTOR_CONTROL_VOLUME == hmiData.hmiMotorControl)
   {
      hmi_write_element(&HmiParams[PARAM_VT]);
   }
}

static void hmi_set_screen_second(void)
{
   int i = 0;
  for (i = SS_ADJUST_ALARMS; i < SS_BACK+1; i++)
  {
     hmi_write_element(&HmiParams[i]);
  }

   DisplayDrv_PositionXY(0, LINE3);
   DisplayDrv_WriteString(HMI_STR_EMPTY);
}

static void hmi_set_screen_settings(HmiParamIdType id)
{
   int i = 0;

   DisplayDrv_PositionXY(HmiParams[id].textX, LINE1);
   DisplayDrv_WriteString(HmiParams[id].text);
   hmi_write_element_value(HmiParams[id].value,
                           HmiParams[id].valueFormat,
                           HmiParams[id].valueX,
                           LINE1);
   DisplayDrv_PositionXY(HmiParams[id].unitX, LINE1);
   DisplayDrv_WriteString(HmiParams[id].unitText);
  
   for (i = 0; i < HMI_SETTING_MAX; i++)
   {
      DisplayDrv_PositionXY(HmiSettings[i].textX, HmiSettings[i].y);
      DisplayDrv_WriteString(HmiSettings[i].text);
   }

   DisplayDrv_PositionXY(HmiParams[id].cursorX, LINE1);
   DisplayDrv_CursorOn();
}

static void hmi_set_screen_control_mode(void)
{
   int i = 0;
   for (i = 0; i < HMI_CONTROL_MODE_MAX; i++)
   {
      DisplayDrv_PositionXY(HmiControlMode[i].textX, HmiControlMode[i].y);
      DisplayDrv_WriteString(HmiControlMode[i].text);
   }
}

static void hmi_set_screen_adjust_alarm_1(void)
{
   int i = 0;
   for (i = 0; i < 4; i++)
   {
      hmi_write_element(&HmiAdjustAlarm[i]);
   }
}

static void hmi_set_screen_adjust_alarm_2(void)
{
   int i = 0;

   for (i = 4; i < 8; i++)
   {
      hmi_write_element(&HmiAdjustAlarm[i]);
   }
}

static void hmi_set_screen_adjust_alarm_3(void)
{
   int i = 0;

   for (i = 8; i < 11; i++)
   {
      hmi_write_element(&HmiAdjustAlarm[i]);
   }
   DisplayDrv_PositionXY(0, LINE3);
   DisplayDrv_WriteString(HMI_STR_EMPTY);
}

static void hmi_set_screen_edit_alarm(HmiParamIdType id)
{
   DisplayDrv_PositionXY(HmiAdjustAlarm[id].textX, LINE1);
   DisplayDrv_WriteString(HmiAdjustAlarm[id].text);
   hmi_write_element_value(HmiAdjustAlarm[id].value,
                           HmiAdjustAlarm[id].valueFormat,
                           HmiAdjustAlarm[id].valueX,
                           LINE1);
   DisplayDrv_PositionXY(HmiAdjustAlarm[id].unitX, LINE1);
   DisplayDrv_WriteString(HmiAdjustAlarm[id].unitText);
  
   DisplayDrv_PositionXY(0, LINE2);
   DisplayDrv_WriteString(HMI_STR_DIVISOR);
   DisplayDrv_PositionXY(0, LINE3);
   DisplayDrv_WriteString(HMI_STR_ACCEPT);
   DisplayDrv_PositionXY(0, LINE4);
   DisplayDrv_WriteString(HMI_STR_CANCEL);

   DisplayDrv_PositionXY(HmiAdjustAlarm[id].cursorX, LINE1);
   DisplayDrv_CursorOn();
}

static void hmi_set_screen_message(HmiMessageTypeId type, AlarmIdType id, uint32_t line4)
{
   char * line[LINE_MAX];

   if (HMI_MSG_ALARM == type)
   {
      line[LINE1] = HMI_STR_ALARM_TITLE;
      line[LINE2] = HmiAlarm[id].line2;
      line[LINE3] = HmiAlarm[id].line3;
      line[LINE4] = (HMI_ALARM_GO_TO_MENU == line4) ? HMI_STR_GO_TO_MENU : HMI_STR_NEXT;
   }
   else
   {
      line[LINE1] = HmiError[id].line1;
      line[LINE2] = HmiError[id].line2;
      line[LINE3] = HmiError[id].line3;
      line[LINE4] = HMI_STR_ACCEPT;
   }

   for (int i = 0 ; i < LINE_MAX ; i++)
   {
      DisplayDrv_PositionXY(0, i);
      DisplayDrv_WriteString(line[i]);
   }

   if ((HMI_MSG_ALARM == type) && (HmiAlarm[id].valueEnable))
   {
      hmi_write_element_value(HmiAlarm[id].value,
                              HmiAlarm[id].format,
                              HmiAlarm[id].valueX,
                              HmiAlarm[id].valueY);
   }
}

static void hmi_set_screen_confirm_stop(void)
{
   int i = 0;

   for (i = 0; i < 3; i++)
   {
      hmi_write_element(&HmiConfirmStop[i]);
   }
   DisplayDrv_PositionXY(0, LINE2);
   DisplayDrv_WriteString(HMI_STR_EMPTY);
}



static void hmi_write_element(HmiElementType * element)
{
   DisplayDrv_PositionXY(element->textX, element->y);
   DisplayDrv_WriteString(element->text);

   if (element->valueEnable)
   {
      hmi_write_element_value(element->value,
                              element->valueFormat,
                              element->valueX,
                              element->y);
   }

   if (element->unitEnable)
   {
      DisplayDrv_PositionXY(element->unitX, element->y);
      DisplayDrv_WriteString(element->unitText);
   }
}

static void hmi_write_element_value_with_cursor(HmiElementType * element, uint8_t x, uint8_t y)
{
   DisplayDrv_CursorOff();
   hmi_write_element_value(element->value,
                           element->valueFormat,
                           x,
                           y);
   DisplayDrv_PositionXY(element->cursorX, y);
   DisplayDrv_CursorOn(); 
}

static void hmi_write_element_value(int32_t value, HmiValueFormatType format, uint8_t x, uint8_t y)
{
   char string[5] = {'\0'};
   DisplayDrv_PositionXY(x, y);

   switch (format)
   {
      case FMT_DOT_DIV10:
         string[0] = '0' + (value/100);
         string[1] = '.';
         string[2] = '0' + ((value/10)%10);
         break;
      case FMT_DOT_DIV1000:
         string[0] = '0' + (value/1000);
         string[1] = '.';
         string[2] = '0' + ((value/100)%10);
         break;
      case FMT_1_DIGIT:
         string[0] = '0' + value;
         break;
      case FMT_2_DIGITS:
         string[0] = (value/10) ? ('0' + (value/10)) : ' ';
         string[1] = '0' + (value%10);
         break;
      case FMT_2_DIGITS_DIV10:
         string[0] = '0' + (value/100);
         string[1] = '0' + ((value/10)%10);
         break;
      case FMT_2_DIGITS_DIV10_INV:
         if (HMI_PARAM_VALUE_NA == value)
         {
            string[0] = '-';
            string[1] = '-';
         }
         else
         {
            string[0] = '0' + (value/100);
            string[1] = '0' + ((value/10)%10);
         }
         break;
      case FMT_3_DIGITS:
         string[0] = '0' + (value/100);
         string[1] = '0' + ((value%100)/10);
         string[2] = '0' + (value%10);
         break;
      case FMT_3_DIGITS_INV:
         if (HMI_PARAM_VALUE_NA == value)
         {
            string[0] = '-';
            string[1] = '-';
            string[2] = '-';
         }
         else
         {
            string[0] = '0' + (value/100);
            string[1] = '0' + ((value%100)/10);
            string[2] = '0' + (value%10);
         }
         break;
      case FMT_4_DIGITS:
         string[0] = '0' + (value/1000);
         string[1] = '0' + ((value%1000)/100);
         string[2] = '0' + ((value/10)%10);
         string[3] = '0' + (value%10);
         break;
      case FMT_CONTROL_MODE:
         string[0] = (HMI_MOTOR_CONTROL_VOLUME == hmiData.hmiMotorControl) ? 'V' : 'P';
         break;
      default:
         break;
   }
   DisplayDrv_WriteString(string);
}

static void hmi_write_control_mode_selector(void)
{
   int lineSelected =    (HMI_MOTOR_CONTROL_VOLUME == hmiData.hmiMotorControl) ? LINE2 : LINE3;
   int lineUnselected =  (HMI_MOTOR_CONTROL_VOLUME != hmiData.hmiMotorControl) ? LINE2 : LINE3;

   DisplayDrv_PositionXY(13, lineSelected);
   DisplayDrv_WriteString("<<<");
   DisplayDrv_PositionXY(13, lineUnselected);
   DisplayDrv_WriteString("   ");
}




static uint32_t hmi_get_next_alarm(HmiSilencedAlarmType * alarms)
{
   int i = 0;
   while (i < AM_ALARMS_MAX)
   {
      if (0 != (alarms->vector & (1 << i)))
      {
         alarms->counter--;
         alarms->vector &= ~(1 << i);
         break;
      }
      i++;
   }
   return i;
}

static void hmi_update_alarm_values(void)
{
   Hmi_GetAlarmPEEPMin((uint32_t *) &HmiAdjustAlarm[AAS1_PEEP_MIN].value);
   Hmi_GetAlarmPIPMin((uint32_t *) &HmiAdjustAlarm[AAS1_PIP_MIN].value);
   Hmi_GetAlarmPIPMax((uint32_t *) &HmiAdjustAlarm[AAS1_PIP_MAX].value);
   Hmi_GetAlarmPIPPEEPDif((uint32_t *) &HmiAdjustAlarm[AAS2_PIP_PEEP].value);
   Hmi_GetAlarmTIError((uint32_t *) &HmiAdjustAlarm[AAS2_TI_MAX].value);
   Hmi_GetAlarmBPMError((uint32_t *) &HmiAdjustAlarm[AAS2_FR_MAX].value);
   Hmi_GetAlarmTidalVolMax((uint32_t *) &HmiAdjustAlarm[AAS3_VT_MAX].value);
   Hmi_GetAlarmTidalVolMin((uint32_t *) &HmiAdjustAlarm[AAS3_VT_MIN].value);
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

