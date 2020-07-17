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
//!   \file       motor_manager.c
//!
//!   \brief      This is the motor manager module implementation file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       23 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "fsm.h"
#include "logger_api.h"
//#include "lpf.h"
#define ARM_MATH_CM3  // Use ARM Cortex M3
#include <arm_math.h>    // Include CMSIS header
#include "adc_drv_api.h"

//********************************************************************
//! @addtogroup motor_manager_imp
//!   @{
//********************************************************************

#include "motor_manager_conf.h"
#include "motor_manager_api.h"
#include "motor_manager_callouts.h"


//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
#define LOG_TAG         "MotorMgr"

#define START_SIG       (USER_SIG)
#define STOP_SIG        (USER_SIG+1)
#define UPDATE_POS_SIG  (USER_SIG+2)

typedef struct motor_mgr_fsm_evt_tag
{
   Signal sig;
   int32_t pos;
   uint32_t speed;
} MotorMgrFsmEventType;

typedef struct hmi_fsm_tag
{
   State state__; /* the current state */
   uint32_t nextDir;
   uint32_t lastTimestamp;
   int32_t lastPosition;
   uint32_t curDriveLvl;

} MotorMgrFsmType;

typedef struct motor_mgr_tag
{
   MotorMgrFsmType   motorFsm;
   arm_pid_instance_q15 pid;
   uint32_t speedSetpoint;
   int32_t distanceSetpoint;
   uint32_t pauseTimeout;
}MotorMgrType;



//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void motor_mgr_fsm_init(void);
static void motor_mgr_fsm_initial(MotorMgrFsmType *me, Event const *e);
static void motor_mgr_fsm_down(MotorMgrFsmType *me, Event const *e);
static void motor_mgr_fsm_pause(MotorMgrFsmType *me, Event const *e);
static void motor_mgr_fsm_up(MotorMgrFsmType *me, Event const *e);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static MotorMgrType motor_mgr_data;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType MotorManager_Init(void)
{
   float32_t coef;

   coef = 0.10f;
   arm_float_to_q15(&coef, &motor_mgr_data.pid.Kp, 1);
   coef = 0.02f;
   arm_float_to_q15(&coef, &motor_mgr_data.pid.Ki, 1);
   coef = 0.01f;
   arm_float_to_q15(&coef, &motor_mgr_data.pid.Kd, 1);
   arm_pid_init_q15(&motor_mgr_data.pid, 1);

   motor_mgr_data.speedSetpoint = 10;
   motor_mgr_data.distanceSetpoint = 400;
   motor_mgr_data.pauseTimeout = 500;

   motor_mgr_fsm_init();

   return E_OK;
}

StatusType MotorManager_Update(void)
{
   FsmDispatch(&motor_mgr_data.motorFsm, (MotorMgrFsmEventType*)&tickEvt);
   return E_OK;
}

extern StatusType MotorManager_Start(void)
{
   MotorMgrFsmEventType evt;


   evt.sig = START_SIG;

   FsmDispatch(&motor_mgr_data.motorFsm, &evt);

   return E_OK;
}

extern StatusType MotorManager_Stop(void)
{
   MotorMgrFsmEventType evt;

   evt.sig = STOP_SIG;

   FsmDispatch(&motor_mgr_data.motorFsm, &evt);

   return E_OK;
}

void MotorManager_UpdatePosAndSpeed(int32_t pos, int32_t deltaPos, uint32_t speed)
{
   MotorMgrFsmEventType evt;

   evt.sig = UPDATE_POS_SIG;
   evt.pos = pos;
   evt.speed = speed;

   FsmDispatch(&motor_mgr_data.motorFsm, &evt);
}

StatusType MotorManager_SetPIDParameters(float32_t kp, float32_t ki, float32_t kd)
{
   arm_float_to_q15(&kp, &motor_mgr_data.pid.Kp, 1);
   arm_float_to_q15(&ki, &motor_mgr_data.pid.Ki, 1);
   arm_float_to_q15(&kd, &motor_mgr_data.pid.Kd, 1);
   arm_pid_init_q15(&motor_mgr_data.pid, 1);

   return E_OK;
}

StatusType MotorManager_SetSetpoint(uint32_t speed, uint32_t distance, uint32_t pauseTimeout)
{
   motor_mgr_data.speedSetpoint = speed;
   motor_mgr_data.distanceSetpoint = (int32_t)distance;
   motor_mgr_data.pauseTimeout = pauseTimeout;

   return E_OK;
}

static void motor_mgr_fsm_init(void)
{
   FsmCtor((Fsm*)&motor_mgr_data.motorFsm, motor_mgr_fsm_initial);
   FsmInit((Fsm*)&motor_mgr_data.motorFsm, (Event*)&entryEvt);
}

static void motor_mgr_fsm_initial(MotorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   switch (e->sig)
   {
      case ENTRY_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "ini", "entry");
         me->lastTimestamp = ticks;
         me->curDriveLvl = 0;
         MotorManager_SetMotorState(MOTOR_MGR_MOTOR_STOP, 0);
         break;

      case START_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "ini", "start");

         FsmTran(me, motor_mgr_fsm_down);
         break;

      case EXIT_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "ini", "exit");
         break;
      case TICK_SIG:
         //Logger_WriteLine("Motor", "s=%s", "ini");
      default:
         break;
   }
}
static void motor_mgr_fsm_down(MotorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   MotorMgrFsmEventType *evt = (MotorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "down", "entry");
         arm_pid_reset_q15(&motor_mgr_data.pid);
         me->lastTimestamp = ticks;
         me->lastPosition = 0; //MotorManager_getMotorPosition();
         me->curDriveLvl = 12;
         MotorManager_SetMotorState(MOTOR_MGR_MOTOR_RUN_DOWN, me->curDriveLvl);
         break;

      case UPDATE_POS_SIG: {
         int16_t speed = (int16_t)evt->speed;
         int32_t error = motor_mgr_data.speedSetpoint - speed;
         error = (0x7FFF * error) / ((int32_t)motor_mgr_data.speedSetpoint);

         int32_t errF = arm_pid_q15(&motor_mgr_data.pid, (q15_t)error);

         errF = (errF * 50) / 16384;
         Logger_WriteLine(LOG_TAG, "s=%lu;e=%lu;p=%d;s=%lu;e=%d;c=%d", 1, evt->sig, evt->pos, evt->speed, error, errF);

         int32_t newDriveLvl;
         newDriveLvl = 12 + errF;
         if (newDriveLvl < 0)
            newDriveLvl = 0;
         if (newDriveLvl > 100)
            newDriveLvl = 100;

         if (newDriveLvl != me->curDriveLvl)
            MotorManager_SetMotorState(MOTOR_MGR_MOTOR_RUN_DOWN, newDriveLvl);

         me->curDriveLvl = newDriveLvl;

         if ((evt->pos - me->lastPosition) > motor_mgr_data.distanceSetpoint)
         {
            me->nextDir = MOTOR_MGR_MOTOR_RUN_UP;
            FsmTran(me, motor_mgr_fsm_pause);
         }

         break;
      }
      case STOP_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "down", "stop");
         FsmTran(me, motor_mgr_fsm_initial);
         break;
      case EXIT_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "down", "exit");
         break;
      case TICK_SIG:

      default:
         break;
   }
}
static void motor_mgr_fsm_pause(MotorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   switch (e->sig)
   {
      case ENTRY_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "pause", "entry");
         me->lastTimestamp = ticks;
         MotorManager_SetMotorState(MOTOR_MGR_MOTOR_BRAKE, 0);
         break;

      case STOP_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "pause", "stop");
         FsmTran(me, motor_mgr_fsm_initial);
         break;

      case EXIT_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "pause", "exit");
         break;

      case TICK_SIG:
         if ((ticks - me->lastTimestamp) >= motor_mgr_data.pauseTimeout)
         {
            if (MOTOR_MGR_MOTOR_RUN_UP == me->nextDir)
            {
               FsmTran(me, motor_mgr_fsm_up);
            }
            else
            {
               FsmTran(me, motor_mgr_fsm_down);
            }
         }
         break;
      default:
         break;
   }
}
static void motor_mgr_fsm_up(MotorMgrFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   MotorMgrFsmEventType *evt = (MotorMgrFsmEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "up", "entry");
         me->lastTimestamp = ticks;
         //me->lastPosition = MotorManager_getMotorPosition();
         MotorManager_SetMotorState(MOTOR_MGR_MOTOR_RUN_UP, 80);
         break;

      case UPDATE_POS_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s;p=%d;s=%lu", "up", "updatePos", evt->pos, evt->speed);
         if ((me->lastPosition - evt->pos) >= 5)
         {
            me->nextDir = MOTOR_MGR_MOTOR_RUN_DOWN;
            FsmTran(me, motor_mgr_fsm_pause);
         }
         break;
      case STOP_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "up", "stop");
         FsmTran(me, motor_mgr_fsm_initial);
         break;
      case EXIT_SIG:
         Logger_WriteLine(LOG_TAG, "s=%s;e=%s", "up", "exit");
         break;
      case TICK_SIG:
         //Logger_WriteLine("Motor", "s=%s", "ini");
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

