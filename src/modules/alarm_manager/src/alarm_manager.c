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
//!   \file       alarm_manager.c
//!
//!   \brief      This is the Alarm Manager module implementation
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       06 May 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "fsm.h"
#include "logger_api.h"

//********************************************************************
//! @addtogroup alarm_manager_imp
//! @{
//********************************************************************

#include "alarm_manager_conf.h"
#include "alarm_manager_api.h"
#include "alarm_manager_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG            "AM"

#define SIG_ALARM_ON       (USER_SIG)
#define SIG_ALARM_OFF      (USER_SIG+1)
#define SIG_SILENCE        (USER_SIG+2)

#define ON                 TRUE
#define OFF                FALSE
#define LED_ON(id)         IOWritePinID((id), IO_ON)
#define LED_OFF(id)        IOWritePinID((id), IO_OFF)
#define LED_TOGGLE(id)     IOTogglePinID((id))
#define BUZZER_ON()        IOWritePinID(AM_BUZZER, IO_ON)
#define BUZZER_OFF()       IOWritePinID(AM_BUZZER, IO_OFF)
#define BUZZER_TOGGLE()    IOTogglePinID(AM_BUZZER)

#define AM_SOUND_INT_CYCLES      (AM_SOUND_INT_MS/ALARM_MGR_UPDATE_TIME_MS)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct alarm_mgr_alarm_tag
{
   AlarmIdType          id;
   AlarmMgrPriorityType priority;
   Bool                 state;
   AlarmMgrStatusType   status;
   uint32_t             ledBlink;
   uint32_t             ledCounter;
   uint32_t             sound;
   uint32_t             soundCounter;
   Bool                 soundPause;
   uint32_t             errorValue;
   uint32_t             silencedCounter;
} AlarmMgrAlarmType;

typedef struct alarm_mgr_fsm_evt_tag
{
   Signal         sig;
   AlarmIdType    alarmId;
} AlarmMgrFsmEventType;

typedef struct alarm_mgr_fsm_tag
{
   State             state__; /* the current state */
   volatile Bool     isInitialized;
} AlarmMgrFsmType;

typedef struct alarm_mgr_tag
{
   AlarmMgrFsmType      alarmMgrFsm;
   AlarmMgrAlarmType *  alarmMgrAlarm;
   AlarmIdType          latchedId;
   uint32_t             queuedCounter;
   AlarmIdType          queuedPriorityId;
   uint32_t             silencedCounter;
   AlarmIdType          silencedPriorityId;
} AlarmMgrType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void alarm_mgr_fsm_init(void);
static void alarm_mgr_fsm_idle(AlarmMgrFsmType *me, Event const *e);
static void alarm_mgr_fsm_latched(AlarmMgrFsmType *me, Event const *e);
static void alarm_mgr_update_sound(void);
static void alarm_mgr_update_leds(void);
static void alarm_mgr_update_led(AlarmIdType id);
inline static bool alarm_mgr_any_latched(void);
inline static bool alarm_mgr_any_silenced(void);
inline static bool alarm_mgr_any_queue(void);
inline static bool alarm_mgr_is_alarm_silenced(AlarmIdType id);
inline static bool alarm_mgr_is_alarm_latched(AlarmIdType id);
inline static bool alarm_mgr_is_alarm_queued(AlarmIdType id);
inline static bool alarm_mgr_is_alarm_off(AlarmIdType id);
inline static bool alarm_mgr_is_alarm_state_on(AlarmIdType id);
inline static bool alarm_mgr_is_alarm_state_off(AlarmIdType id);
static void alarm_mgr_set_latched(AlarmIdType id);
static void alarm_mgr_replace_latched(AlarmIdType id);
static void alarm_mgr_reset_latched(AlarmIdType id);
static void alarm_mgr_add_to(AlarmMgrStatusType status, AlarmIdType id);
static void alarm_mgr_remove_from(AlarmMgrStatusType status, AlarmIdType id);
static AlarmIdType alarm_mgr_get_more_priority_id(AlarmMgrStatusType status);
inline AlarmIdType alarm_mgr_which_is_more_prior(AlarmIdType id1, AlarmIdType id2);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************
#undef X
 #define X(a,b,c,d) {a,b,OFF,AM_STATUS_OFF,c,0,d,0,FALSE,0,0},
 AlarmMgrAlarmType AlarmMgrAlarm[] = {
    AM_ALARMS_CFG
 };

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static AlarmMgrType alarmMgrData = {0};

//********************************************************************
// Function Definitions
//********************************************************************
StatusType AlarmMgr_Init(void)
{
   alarmMgrData.alarmMgrAlarm = AlarmMgrAlarm;
   alarmMgrData.latchedId = AM_ALARM_NONE;
   alarmMgrData.queuedPriorityId = AM_ALARM_NONE;
   alarmMgrData.queuedCounter = 0;
   alarmMgrData.silencedPriorityId = AM_ALARM_NONE;
   alarmMgrData.silencedCounter = 0;

   alarm_mgr_fsm_init();

   return E_OK;
}

void AlarmMgr_Update(void)
{
   AlarmMgrFsmEventType evt;
   int id = 0;

   if (alarm_mgr_any_silenced())
   {
      for (id = 0; id < AM_ALARMS_MAX; id++)
      {
         if (alarm_mgr_is_alarm_silenced(id))
         {
            AlarmMgrAlarm[id].silencedCounter++;
            if (AlarmMgrAlarm[id].silencedCounter > ALARM_MGR_SILENCED_TIMEOUT_CYCLES)
            {
               alarm_mgr_remove_from(AM_STATUS_SILENCED, id);
               evt.alarmId = id;
               evt.sig = SIG_ALARM_ON;
               FsmDispatch(&alarmMgrData.alarmMgrFsm, &evt);
            }
         }
      }
   }

   FsmDispatch(&alarmMgrData.alarmMgrFsm, (AlarmMgrFsmEventType*)&tickEvt);
}

StatusType AlarmMgr_SetAlarm(AlarmIdType id, bool status, void * errorValue)
{
   if ((FALSE == alarmMgrData.alarmMgrFsm.isInitialized) || (AM_ALARMS_MAX <= id))
      return E_ERROR;
      
   AlarmMgrFsmEventType evt;

   AlarmMgrAlarm[id].state = (status) ? ON : OFF;
   evt.alarmId = id;
   evt.sig = (status) ? SIG_ALARM_ON : SIG_ALARM_OFF;
   if (true == status)
   {
      AlarmMgrAlarm[id].errorValue = (uint32_t) errorValue;
   }
   FsmDispatch(&alarmMgrData.alarmMgrFsm, &evt);

   return E_OK;
}

StatusType AlarmMgr_Silence(void)
{
   if (FALSE == alarmMgrData.alarmMgrFsm.isInitialized)
      return E_ERROR;

   AlarmMgrFsmEventType evt;
   evt.sig = SIG_SILENCE;
   FsmDispatch(&alarmMgrData.alarmMgrFsm, &evt);

   return E_OK;
}

StatusType AlarmMgr_PauseAudio(void)
{
   AlarmMgrAlarm[alarmMgrData.latchedId].soundPause = TRUE;
   return E_OK;
}

void AlarmMgr_GetSilencedAlarms(uint32_t * alarmVector, uint32_t * alarmQuantity)
{
   int i = 0;
   *alarmQuantity = alarmMgrData.silencedCounter;

   if (alarm_mgr_any_silenced())
   {
      for (i = 0; i < AM_ALARMS_MAX; i++)
      {
         if (alarm_mgr_is_alarm_silenced(i))
         {
            *alarmVector |= (1 << i);
         }
      }
   }
}

//********************************************************************
// Finite State Machine Functions
//********************************************************************
static void alarm_mgr_fsm_init(void)
{
   FsmCtor((Fsm*)&alarmMgrData.alarmMgrFsm, alarm_mgr_fsm_idle);
   FsmInit((Fsm*)&alarmMgrData.alarmMgrFsm, (Event*)&entryEvt);
   alarmMgrData.alarmMgrFsm.isInitialized = TRUE;
}

static void alarm_mgr_fsm_idle(AlarmMgrFsmType *me, Event const *e)
{
   AlarmMgrFsmEventType *evt = (AlarmMgrFsmEventType*) e;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s", "idle", "entry");
         break;
      case SIG_ALARM_ON:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s", "idle", "alarm_on");
         alarm_mgr_set_latched(evt->alarmId);
         FsmTran(me, alarm_mgr_fsm_latched);
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s", "idle", "exit");
         break;
      default:
         break;
   }
}

static void alarm_mgr_fsm_latched(AlarmMgrFsmType *me, Event const *e)
{
   AlarmMgrFsmEventType *evt = (AlarmMgrFsmEventType*) e;
   AlarmIdType alarmToLatch;

   switch (e->sig)
   {
      case ENTRY_SIG:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s", "latch", "entry");
         break;
      case SIG_SILENCE:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s", "latch", "silence");
         if (alarm_mgr_any_latched() == false)
         {
            break;
         }
         if (alarm_mgr_is_alarm_state_on(alarmMgrData.latchedId))
         {
            alarm_mgr_add_to(AM_STATUS_SILENCED, alarmMgrData.latchedId);
            AlarmMgr_UpdateSilencedAlarmCounter(alarmMgrData.silencedCounter);
         }
         else
         {
            AlarmMgrAlarm[alarmMgrData.latchedId].status = AM_STATUS_OFF;
         }
         if (alarm_mgr_any_queue())
         {
            alarmToLatch = alarm_mgr_get_more_priority_id(AM_STATUS_QUEUED);
            alarm_mgr_remove_from(AM_STATUS_QUEUED, alarmToLatch);
            alarm_mgr_set_latched(alarmToLatch);
         }
         else
         {
            alarm_mgr_reset_latched(alarmMgrData.latchedId);
         }
         break;
      case SIG_ALARM_ON:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s;id=%d", "latch", "alarm_on", evt->alarmId);
         if (alarm_mgr_is_alarm_silenced(evt->alarmId) || alarm_mgr_is_alarm_queued(evt->alarmId))
         {
            break;
         }
         else if (alarm_mgr_is_alarm_latched(evt->alarmId))
         {
            AlarmMgr_SetAlarmScreen(evt->alarmId, true, AlarmMgrAlarm[evt->alarmId].errorValue,
                                    alarm_mgr_any_queue() ? AM_NOT_EMPTY_QUEUE : AM_EMPTY_QUEUE);
         }
         else if (alarm_mgr_any_latched())
         {
            if ( AlarmMgrAlarm[alarmMgrData.latchedId].id < AlarmMgrAlarm[evt->alarmId].id )
            {
               alarm_mgr_add_to(AM_STATUS_QUEUED, evt->alarmId);
               AlarmMgr_SetAlarmScreen(alarmMgrData.latchedId, true, AlarmMgrAlarm[alarmMgrData.latchedId].errorValue,
                                       alarm_mgr_any_queue() ? AM_NOT_EMPTY_QUEUE : AM_EMPTY_QUEUE);
            }
            else
            {
               alarm_mgr_add_to(AM_STATUS_QUEUED, alarmMgrData.latchedId);
               alarm_mgr_replace_latched(evt->alarmId);
            }
         }
         else
         {
            alarm_mgr_set_latched(evt->alarmId);
         }
         break;
      case SIG_ALARM_OFF:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s;id=%d", "latch", "alarm_off", evt->alarmId);
         if (alarm_mgr_is_alarm_silenced(evt->alarmId))
         {
            alarm_mgr_remove_from(AM_STATUS_SILENCED, evt->alarmId);
            AlarmMgr_UpdateSilencedAlarmCounter(alarmMgrData.silencedCounter);
            AlarmMgrAlarm[evt->alarmId].status = AM_STATUS_OFF;
         }
         if ((alarm_mgr_any_latched() == false) && (alarm_mgr_any_queue() == false) && (alarm_mgr_any_silenced() == false))
         {
            FsmTran(me, alarm_mgr_fsm_idle);
         }
         break;
      case TICK_SIG:
         alarm_mgr_update_leds();
         alarm_mgr_update_sound();
         break;
      case EXIT_SIG:
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "s=%s;e=%s", "latch", "exit");
         BUZZER_OFF();
         LED_OFF(AM_LED_ALARM);
         LED_OFF(AM_LED_BACKUP_BATTERY);
         LED_OFF(AM_EXT_LIGHT);
         break;
      default:
         break;
   }
}

//********************************************************************
// Private Functions
//********************************************************************
static void alarm_mgr_update_leds(void)
{
   Bool alarmLedHandled = FALSE;
   for (int i = 0; i < AM_ALARMS_MAX ; i++)
   {
      if (alarm_mgr_is_alarm_latched(i) || alarm_mgr_is_alarm_silenced(i))
      {
         alarm_mgr_update_led(i);
         alarmLedHandled = TRUE;
         break;
      }
   }
   if (FALSE == alarmLedHandled)
   {
      LED_OFF(AM_LED_ALARM);
      LED_OFF(AM_EXT_LIGHT);
   }
   
   if (alarm_mgr_is_alarm_latched(AM_BATTERY_MODE) || 
      alarm_mgr_is_alarm_silenced(AM_BATTERY_MODE) ||
      alarm_mgr_is_alarm_queued(AM_BATTERY_MODE))
   {
      LED_ON(AM_LED_BACKUP_BATTERY);
   }
   else
   {
      LED_OFF(AM_LED_BACKUP_BATTERY);
   }
}

// TODO: Include this variable in the function, now its outside for debugging purpose
static uint32_t soundPauseCounter = 0;
static void alarm_mgr_update_sound(void)
{
#if (true == AM_DEBUG_NO_BUZZER)
   static uint32_t informed = 0;
#endif


   if (AM_ALARM_NONE == alarmMgrData.latchedId)
   {
      BUZZER_OFF();
      return;
   }

   if (AlarmMgrAlarm[alarmMgrData.latchedId].soundPause)
   {
      if (0 == soundPauseCounter)
      {
         BUZZER_OFF();
      }
      else if (ALARM_MGR_SILENCED_TIMEOUT_CYCLES <= soundPauseCounter)
      {
         soundPauseCounter = 0;
         AlarmMgrAlarm[alarmMgrData.latchedId].soundPause = FALSE;
      }
      soundPauseCounter++;
   }
   else
   {
#if (true == AM_DEBUG_NO_BUZZER)
      if (informed == 500) {
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "--- SOUND:%d", AlarmMgrAlarm[alarmMgrData.latchedId].sound);
         informed = 0;
      } else {
         informed++;
      }
#elif (false == AM_DEBUG_NO_BUZZER)
      switch (AlarmMgrAlarm[alarmMgrData.latchedId].sound)
      {
         case AM_SOUND_OFF:
            break;
         case AM_SOUND_ON:
            BUZZER_ON();
            break;
         case AM_SOUND_INT_MS:
            if (AlarmMgrAlarm[alarmMgrData.latchedId].soundCounter == AM_SOUND_INT_CYCLES) {
               BUZZER_TOGGLE();
               AlarmMgrAlarm[alarmMgrData.latchedId].soundCounter = 0;
            }
            AlarmMgrAlarm[alarmMgrData.latchedId].soundCounter++;
            break;
         default:
            break;
      }
#endif
      soundPauseCounter = 0;
   }
}

static void alarm_mgr_update_led(AlarmIdType id)
{
   if (id >= AM_ALARMS_MAX)
      return;
   
   switch (AlarmMgrAlarm[id].priority)
   {
      case AM_PRIORITY_HIGH:
         if (AlarmMgrAlarm[id].ledCounter == AM_LED_BLINK_FAST_CYCLES)
         {
            LED_TOGGLE(AM_LED_ALARM);
            LED_TOGGLE(AM_EXT_LIGHT);
            AlarmMgrAlarm[id].ledCounter = 0;
         }
         AlarmMgrAlarm[id].ledCounter++;
         break;
      case AM_PRIORITY_MIDDLE:
         if (AlarmMgrAlarm[id].ledCounter == AM_LED_BLINK_SLOW_CYCLES)
         {
            LED_TOGGLE(AM_LED_ALARM);
            AlarmMgrAlarm[id].ledCounter = 0;
         }
         AlarmMgrAlarm[id].ledCounter++;
         break;
      case AM_PRIORITY_LOW:
         LED_ON(AM_LED_ALARM);
         break;
      default:
         break;
   }
}


inline static bool alarm_mgr_any_latched(void)
{
   return (alarmMgrData.latchedId != AM_ALARM_NONE);
}

inline static bool alarm_mgr_any_silenced(void)
{
  return (alarmMgrData.silencedCounter != 0);
}

inline static bool alarm_mgr_any_queue(void)
{
   return (alarmMgrData.queuedCounter != 0);
}


inline static bool alarm_mgr_is_alarm_silenced(AlarmIdType id)
{
   return (id >= AM_ALARMS_MAX) ? false : (AlarmMgrAlarm[id].status == AM_STATUS_SILENCED);
}

inline static bool alarm_mgr_is_alarm_latched(AlarmIdType id)
{
   return (id >= AM_ALARMS_MAX) ? false : (AlarmMgrAlarm[id].status == AM_STATUS_LATCHED);
}

inline static bool alarm_mgr_is_alarm_queued(AlarmIdType id)
{
   return (id >= AM_ALARMS_MAX) ? false : (AlarmMgrAlarm[id].status == AM_STATUS_QUEUED);
}

inline static bool alarm_mgr_is_alarm_off(AlarmIdType id)
{
   return (id >= AM_ALARMS_MAX) ? false : (AlarmMgrAlarm[id].status == AM_STATUS_OFF);
}

inline static bool alarm_mgr_is_alarm_state_on(AlarmIdType id)
{
   return (id >= AM_ALARMS_MAX) ? false : (AlarmMgrAlarm[id].state == ON);
}

inline static bool alarm_mgr_is_alarm_state_off(AlarmIdType id)
{
   return (id >= AM_ALARMS_MAX) ? false : (AlarmMgrAlarm[id].state == OFF);
}


static void alarm_mgr_set_latched(AlarmIdType id)
{
   AlarmMgr_SetAlarmScreen(id, true, AlarmMgrAlarm[id].errorValue,
                           alarm_mgr_any_queue() ? AM_NOT_EMPTY_QUEUE : AM_EMPTY_QUEUE);
   AlarmMgrAlarm[id].status = AM_STATUS_LATCHED;
   AlarmMgrAlarm[id].soundPause = FALSE;
   alarmMgrData.latchedId = id;
}

static void alarm_mgr_replace_latched(AlarmIdType id)
{
   alarm_mgr_set_latched(id);
}

static void alarm_mgr_reset_latched(AlarmIdType id)
{
   AlarmMgr_SetAlarmScreen(id, false, 0,
                           alarm_mgr_any_queue() ? AM_NOT_EMPTY_QUEUE : AM_EMPTY_QUEUE);
   alarmMgrData.latchedId = AM_ALARM_NONE;
}

static void alarm_mgr_add_to(AlarmMgrStatusType status, AlarmIdType id)
{
   AlarmIdType * priority = NULL;

   AlarmMgrAlarm[id].status = status;

   if (status == AM_STATUS_QUEUED)
   {
      alarmMgrData.queuedCounter++;
      priority = &alarmMgrData.queuedPriorityId;
   }
   else if (status == AM_STATUS_SILENCED)
   {
      alarmMgrData.silencedCounter++;
      priority = &alarmMgrData.silencedPriorityId;
      AlarmMgrAlarm[id].silencedCounter = 0;
   }

   if (*priority == AM_ALARM_NONE)
   {
      *priority = id;
   }
   else
   {
      *priority = (alarm_mgr_which_is_more_prior(*priority, id));
   }
}

static void alarm_mgr_remove_from(AlarmMgrStatusType status, AlarmIdType id)
{
   uint32_t * counter = NULL;
   AlarmIdType * priority = NULL;

   AlarmMgrAlarm[id].status = AM_STATUS_OFF;

   if (status == AM_STATUS_QUEUED)
   {
      counter = &alarmMgrData.queuedCounter;
      priority = &alarmMgrData.queuedPriorityId;
   }
   else if (status == AM_STATUS_SILENCED)
   {
      counter = &alarmMgrData.silencedCounter;
      priority = &alarmMgrData.silencedPriorityId;
      AlarmMgrAlarm[id].silencedCounter = 0;
   }

   (*counter)--;

   if (*counter != 0)
   {
      *priority = alarm_mgr_get_more_priority_id(status);
   }
   else
   {
      *priority = AM_ALARM_NONE;
   }
}


static AlarmIdType alarm_mgr_get_more_priority_id(AlarmMgrStatusType status)
{
   int i = 0;
   AlarmIdType id = AM_ALARM_NONE;

   for (i = 0; i < AM_ALARMS_MAX; i++)
   {
      if (AlarmMgrAlarm[i].status == status)
      {
         if (id == AM_ALARM_NONE)
         {
            id = i;
         }
         else if (AlarmMgrAlarm[i].id < AlarmMgrAlarm[id].id)
         {
            id = i;
         }
      }
   }

   return id;
}

inline AlarmIdType alarm_mgr_which_is_more_prior(AlarmIdType id1, AlarmIdType id2)
{
   return (AlarmMgrAlarm[id1].priority > AlarmMgrAlarm[id2].priority) ? id2 : id1;
}

#define STR_OFF         "OFF"
#define STR_LATCHED     "LATCHED"
#define STR_QUEUED      "QUEUED"
#define STR_SILENCED    "SILENCED"
#define STR_ON          "ON "
#define STR_FALSE       "FALSE"
#define STR_TRUE        "TRUE "

#define STR_STATE(i)       (OFF == AlarmMgrAlarm[i].state) ? STR_OFF : STR_ON
#define STR_SOUND_PAUSE(i) (FALSE == AlarmMgrAlarm[i].soundPause) ? STR_FALSE : STR_TRUE

void AlarmMgr_AlarmStatus(void)
{
   LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "------------------------------------------------");
   LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "latched:%d | queuedCounter:%d | silencedCounter:%d", 
                     alarmMgrData.latchedId, alarmMgrData.queuedCounter, alarmMgrData.silencedCounter);
   for (int i = 0; i < AM_ALARMS_MAX; i++)
   {
      if (AlarmMgrAlarm[i].status == AM_STATUS_OFF)
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "id:%d | %s | %s | %s", AlarmMgrAlarm[i].id, STR_STATE(i), STR_OFF, STR_SOUND_PAUSE(i));
      if (AlarmMgrAlarm[i].status == AM_STATUS_LATCHED)
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "id:%d | %s | %s | %s", AlarmMgrAlarm[i].id, STR_STATE(i), STR_LATCHED, STR_SOUND_PAUSE(i));
      if (AlarmMgrAlarm[i].status == AM_STATUS_QUEUED)
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "id:%d | %s | %s | %s", AlarmMgrAlarm[i].id, STR_STATE(i), STR_QUEUED, STR_SOUND_PAUSE(i));
      if (AlarmMgrAlarm[i].status == AM_STATUS_SILENCED)
         LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "id:%d | %s | %s | counter:%d [sec] | %s", AlarmMgrAlarm[i].id, STR_STATE(i), 
                                                                                 STR_SILENCED, AlarmMgrAlarm[i].silencedCounter/100,
                                                                                 STR_SOUND_PAUSE(i));
   }
   LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "AudioPauseCounter: %d", soundPauseCounter/100);
   LOG_PRINT_VER(DEBUG_AM, LOG_TAG, "------------------------------------------------");
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

