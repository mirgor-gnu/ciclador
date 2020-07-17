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
//!   \file       motor_fsm.c
//!
//!   \brief      This is the motor state machine implementation file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "board_hw_io.h"
#include "logger_api.h"
#include "lpf.h"

//********************************************************************
//! @addtogroup motor_drv_imp
//!   @{
//********************************************************************

#include "motor_drv_conf.h"
#include "motor_drv_api.h"
#include "motor_drv_callouts.h"
#include "motor_drv.h"
#include "motor_fsm.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG   "MotorDrv"

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct motor_fsm_state_map_tag
{
   uint32_t stateId;
   void (*stateFunc)(Fsm *, Event const *);
   //void (*stateFunc) (MotorFsmType *me, Event const *e);
}MotorFsmMapType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void motor_fsm_STATE_INITIAL(MotorFsmType *me, Event const *e);
static void motor_fsm_STATE_HOME(MotorFsmType *me, Event const *e);
//static void motor_fsm_start(MotorFsmType *me, Event const *e);
static void motor_fsm_STATE_RUN(MotorFsmType *me, Event const *e);
static void motor_fsm_STATE_STOP(MotorFsmType *me, Event const *e);
static void motor_fsm_STATE_ERROR(MotorFsmType *me, Event const *e);

void motor_stop(MotorFsmType *me, MotorStopType stopType);
void motor_set_drive_level(MotorFsmType *me, MotorDirType dir, uint32_t level);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************
static const MotorFsmMapType motor_fsm_map[] = {
#define X(a) {a, (void (*)(Fsm *, Event const *))motor_fsm_##a},
      MOTOR_FSM_STATES
#undef X
};
//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************

extern MotorDrvType motor_drv_data;

//********************************************************************
// Function Definitions
//********************************************************************

MotorFsmStateType motor_fsm_get_state(MotorDrvType *pDrvData)
{
   uint32_t i,tableSize;
   MotorFsmType *fsm;

   fsm = (MotorFsmType*) pDrvData->pFsm;
   tableSize = sizeof(motor_fsm_map) / sizeof(motor_fsm_map[0]);

   for(i=0; i<tableSize; i++)
   {
      if (motor_fsm_map[i].stateFunc == fsm->state__)
      {
         break;
      }
   }
   if (i == tableSize)
   {
      return -1;
   }

   return motor_fsm_map[i].stateId;
}

StatusType motor_fsm_init(MotorDrvType *pDrvData)
{
   if (NULL == pDrvData)
   {
      return E_ERROR;
   }

   ((MotorFsmType*)pDrvData->pFsm)->pDrvData = pDrvData;
   FsmCtor((Fsm*)pDrvData->pFsm, motor_fsm_STATE_INITIAL);
   FsmInit((Fsm*)pDrvData->pFsm, (Event*)&entryEvt);

   return E_OK;
}

void motor_fsm_dispatch(MotorDrvType *pDrvData, const MotorEventType *e)
{
   FsmDispatch((Fsm *)pDrvData->pFsm, (Event *)e);
}

void motor_fsm_transition(MotorDrvType *pDrvData, State *newState)
{

}

void motor_stop(MotorFsmType *me, MotorStopType stopType)
{
   if (MOTOR_STOP_NORMAL == stopType)
   {
      IOWritePinID(IO_MOTOR_DIRA, IO_OFF);
      IOWritePinID(IO_MOTOR_DIRB, IO_OFF);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_A, 0);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_B, 0);
   }
   else
   {
      IOWritePinID(IO_MOTOR_DIRA, IO_ON);
      IOWritePinID(IO_MOTOR_DIRB, IO_ON);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_A, 100);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_B, 100);
   }
}

void motor_set_drive_level(MotorFsmType *me, MotorDirType dir, uint32_t level)
{
   if (MOTOR_DIR_CW == dir)
   {
      // the motor has both terminals in hi-z
      IOWritePinID(IO_MOTOR_DIRA, IO_OFF);
      IOWritePinID(IO_MOTOR_DIRB, IO_ON);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_A, level);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_B, 100);
   }
   else
   {
      // the motor has both terminals (short-circuited) to ground
      IOWritePinID(IO_MOTOR_DIRA, IO_ON);
      IOWritePinID(IO_MOTOR_DIRB, IO_OFF);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_A, 100);
      MOTOR_DRV_SET_PWM(me->pDrvData, MOTOR_DRV_CHANNEL_B, level);
   }
}

static void motor_fsm_STATE_INITIAL(MotorFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   MotorEventType *evt = (MotorEventType*) e;
   if (e->sig != TICK_SIG)
      LOG_PRINT_INFO(DEBUG_MOTOR, "Motor", "s=%lu;d=%lu;l=%ld;e=%lu", motor_fsm_get_state(me->pDrvData), me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);

   switch (e->sig)
   {
      case ENTRY_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "ini", "entry");
         me->autoRestart = 0;
         me->lastTimestamp = ticks;

         // start searching for home position
         uint32_t homeSwitchState = IOReadPinID(MOTOR_DRV_HOME_SWITCH_PIN);
         if (0 == homeSwitchState)
         {
            //the home position is behind us
            motor_set_drive_level(me, MOTOR_DIR_CW, MOTOR_DRV_HOMING_DRIVE_LEVEL);
         }
         else
         {
            //the home position is ahead us
            motor_set_drive_level(me, MOTOR_DIR_CCW, MOTOR_DRV_HOMING_DRIVE_LEVEL);
         }
         break;

      case HOME_SIG:
         motor_stop(me, MOTOR_STOP_BRAKE);
         // check the new state of the HOME sensor
         if (0 != evt->data)
         {
            FsmTran(me, motor_fsm_STATE_HOME);
         }
         else
         {
            FsmTran(me, motor_fsm_STATE_INITIAL);
         }
         break;
      case EXIT_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "ini", "exit");
         break;
      case TICK_SIG:
         //Logger_WriteLine("Motor", "s=%s", "ini");
         if ((ticks - me->lastTimestamp) >= MOTOR_DRV_HOMING_TIMEOUT_MILLIS)
         {
            //we did not find the home position.
            MotorDrv_OnError(MOTOR_DRV_ERROR_HOMENOTFOUND);
            FsmTran(me, motor_fsm_STATE_ERROR);
         }
         break;
      default:
         break;
   }
   //   uint32_t stateId = 0;
   //   if (e->sig != TICK_SIG)
   //      Logger_WriteLine("Motor", "s=%lu;d=%lu;l=%lu;e=%lu", stateId, me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);
}

static void motor_fsm_STATE_HOME(MotorFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   if (e->sig != TICK_SIG)
      LOG_PRINT_INFO(DEBUG_MOTOR, "Motor", "s=%lu;d=%lu;l=%ld;e=%lu", motor_fsm_get_state(me->pDrvData), me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);

   switch (e->sig)
   {
      case ENTRY_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "ini", "entry");
         me->autoRestart = 0;
         me->lastTimestamp = ticks;
         //reset motor position at home
         MotorDrv_ResetPosition();
         //update interval variables state;
         me->pDrvData->curDir = 0;
         me->pDrvData->curDriveLvl = 0;
         me->pDrvData->curDistance = 0;
         me->lastPosition = MotorDrv_GetPosition();

         motor_stop(me, MOTOR_STOP_BRAKE);
         break;

      case START_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "ini", "start");
         if (me->pDrvData->newDistance > 0)
         {
            FsmTran(me, motor_fsm_STATE_RUN);
         }
         // if we are at home position we will not move when asked to to go to home position
         /*
         uint32_t homeSwitchState = IOReadPinID(MOTOR_DRV_HOME_SWITCH_PIN);
         if ((me->pDrvData->newDistance < 0) && (homeSwitchState != 0))
         {
            FsmTran(me, motor_fsm_STATE_RUN);
         }
         if ((me->pDrvData->newDistance < 0) && (homeSwitchState == 0))
         {
            FsmTran(me, motor_fsm_STATE_HOME);
         }
          */
         break;
      case HOME_SIG:
         //we should never receive this event while in this state
         //FsmTran(me, motor_fsm_STATE_ERROR);
         break;
      case EXIT_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "ini", "exit");
         break;
      case TICK_SIG:
         //Logger_WriteLine("Motor", "s=%s", "ini");
         break;
      default:
         break;
   }
   //   uint32_t stateId = 0;
   //   if (e->sig != TICK_SIG)
   //      Logger_WriteLine("Motor", "s=%lu;d=%lu;l=%ld;e=%lu", stateId, me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);
}
/*
static void motor_fsm_start(MotorFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   MotorEventType *evt = (MotorEventType*) e;
   switch (e->sig)
   {
      case ENTRY_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "start", "entry");
         me->lastTimestamp = ticks;

         // we always start at 100%;
         motor_set_drive_level(me, me->pDrvData->newDir, me->pDrvData->newDriveLvl);

         // we update only the direction, because we the final drive level is defined at run state
         me->pDrvData->curDir = me->pDrvData->newDir;
         me->pDrvData->curDriveLvl = me->pDrvData->newDriveLvl;

         break;
      case STOP_SIG:
         me->autoRestart = 0;
         me->stopType = evt->data;
         FsmTran(me, motor_fsm_stop);
         break;
      case EXIT_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "start", "exit");
         break;

      case TICK_SIG:
         if ((ticks - me->lastTimestamp) >= MOTOR_DRV_STARTUP_TIME_MS)
         {
            FsmTran(me, motor_fsm_run);
         }
         break;
      default:
         break;
   }

   uint32_t stateId = 1;
   if (e->sig != TICK_SIG)
      Logger_WriteLine("Motor", "s=%lu;d=%lu;l=%lu;e=%lu", stateId, me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);
}
 */

static inline uint32_t getPWMFromSpeed(uint32_t speed)
{
   if (0 == speed)
      return 0;

   return ((433 * speed + 9600) / 1000);
}

static void motor_fsm_STATE_RUN(MotorFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();
   MotorEventType *evt = (MotorEventType*) e;
   if (e->sig != TICK_SIG)
      LOG_PRINT_INFO(DEBUG_MOTOR, "Motor", "s=%lu;d=%lu;l=%ld;e=%lu", motor_fsm_get_state(me->pDrvData), me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);

   switch (e->sig)
   {
      case ENTRY_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "run", "entry");
         me->lastTimestamp = ticks;

         //reset pid controller
         arm_pid_reset_q15(&me->pDrvData->pid);

         // if we are at home position we will not move when asked to to go to home position
         uint32_t homeSwitchState = IOReadPinID(MOTOR_DRV_HOME_SWITCH_PIN);
         if ((me->pDrvData->newDistance < 0) && (homeSwitchState != 0))
         {

            FsmTran(me, motor_fsm_STATE_HOME);
            MotorDrv_OnMoveComplete();
         }
         else
         {

            //check if we have to move by drive level or speed
            if (me->pDrvData->newDriveLvl >= 0)
            {
               // we have to move at the specified drive level
               motor_set_drive_level(me, me->pDrvData->newDir, me->pDrvData->newDriveLvl);
            }
            else
            {
               // we have to move controlling speed
               motor_set_drive_level(me, me->pDrvData->newDir, getPWMFromSpeed(me->pDrvData->newSpeed));
            }

            //update interval variables state;
            me->pDrvData->curDir = me->pDrvData->newDir;
            me->pDrvData->curDriveLvl = me->pDrvData->newDriveLvl;
            me->pDrvData->curDistance = me->pDrvData->newDistance;
            me->lastPosition = MotorDrv_GetPosition();
         }
         break;
      case START_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "run", "start");
         // first check if we need to continue rotating in this direction
         if (me->pDrvData->newDir != me->pDrvData->curDir)
         {
            //we need to change direction so we need to do the complete cycle
            me->autoRestart = 1;
            me->stopType = MOTOR_STOP_NORMAL;
            FsmTran(me, motor_fsm_STATE_STOP);
         }
         else if (me->pDrvData->newDriveLvl != me->pDrvData->curDriveLvl)
         {
            //we need to keep the rotating in the same direction but change the drive level
            FsmTran(me, motor_fsm_STATE_RUN);
            //motor_set_drive_level(me, me->pDrvData->curDir, me->pDrvData->newDriveLvl);

            //update current level
            //me->pDrvData->curDriveLvl = me->pDrvData->newDriveLvl;
         }
         break;

      case UPDATE_DRV_SIG:
      {
         //we need to update our drive level but first check if we are in
         //speed controlled mode
         if (me->pDrvData->newSpeed < 0)
         {
            // we have to move at the specified drive level
            motor_set_drive_level(me, me->pDrvData->newDir, me->pDrvData->newDriveLvl);

            //update interval variables state;
            me->pDrvData->curDriveLvl = me->pDrvData->newDriveLvl;
            me->pDrvData->curDir = me->pDrvData->newDir;
         }
         break;
      }
      case UPDATE_POS_SIG: {
         if (0 > me->pDrvData->newDriveLvl)
         {
            //int16_t speed = (int16_t)evt->speed;
            int32_t speederror = me->pDrvData->newSpeed - evt->speed; //speed;
            speederror = (0x4000 * speederror) / me->pDrvData->newSpeed;
            speederror = __SSAT(speederror,16);
            //error = 0x7FFF;
            /*
         int16_t error_q15 = error;

         lpf_writeInput(me->pDrvData->lpf, error_q15 );
         error = (lpf_readOutput( me->pDrvData->lpf)) << 0;
             */
            int32_t errF = arm_pid_q15(&me->pDrvData->pid, (q15_t)speederror);

            errF = (errF * 50) / 16384;
            //         errF = ((errF << 1) * 100) / 0x7FFF;
            LOG_PRINT_INFO(DEBUG_MOTOR_DRV, LOG_TAG, "s=%lu;e=%lu;p=%d;s=%lu;e=%d;c=%d", 1, evt->sig, evt->pos, evt->speed, speederror, errF);

            int32_t newDriveLvl;
            newDriveLvl = getPWMFromSpeed(me->pDrvData->newSpeed) + errF;
            if (newDriveLvl < 0)
               newDriveLvl = 0;
            if (newDriveLvl > 100)
               newDriveLvl = 100;

            //check if we need to control the speed
            if ((0 >= me->pDrvData->newDriveLvl) &&
                  (newDriveLvl != me->pDrvData->curDriveLvl))
            {
               // we need to control the speed, so we update the drive level based on controller output
               motor_set_drive_level(me, me->pDrvData->curDir, newDriveLvl);
               me->pDrvData->curDriveLvl = newDriveLvl;
            }
         }

         //check if we need to stop by traveled distance
         if (me->pDrvData->newDistance > 0)
         {
            // do we traveled the required distance?
            uint32_t dist = (evt->pos > me->lastPosition)? (evt->pos - me->lastPosition):(me->lastPosition-evt->pos);
            if (dist > me->pDrvData->newDistance)
            {
               // we traveled the required distance. we now stop
               me->autoRestart = 0;
               me->stopType = MOTOR_STOP_BRAKE;
               FsmTran(me, motor_fsm_STATE_STOP);
               MotorDrv_OnMoveComplete();
            }
         }

         break;
      }
      case HOME_SIG:
         //check if we need to stop when reaching home position
         if (me->pDrvData->newDistance < 0)
         {
            // we reached home position. we now stop
            //me->autoRestart = 0;
            //me->stopType = MOTOR_STOP_BRAKE;
            FsmTran(me, motor_fsm_STATE_HOME);
            MotorDrv_OnMoveComplete();
         }
         break;
      case GO_HOME_SIG:
         FsmTran(me, motor_fsm_STATE_INITIAL);
         break;
      case STOP_SIG:
         me->autoRestart = 0;
         me->stopType = ((MotorEventType*)e)->data;
         FsmTran(me, motor_fsm_STATE_STOP);
         break;
      case EXIT_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "run", "exit");
         break;
      case TICK_SIG:

         break;
      default:
         break;
   }
}

static void motor_fsm_STATE_STOP(MotorFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();

   switch (e->sig)
   {
      case ENTRY_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "stop", "entry");
         me->lastTimestamp = ticks;

         //stop motor
         motor_stop(me, me->stopType);
         me->pDrvData->curDriveLvl = 0;

         break;
      case START_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "ini", "start");
         FsmTran(me, motor_fsm_STATE_RUN);
         break;
      case EXIT_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "stop", "exit");
         break;
      case GO_HOME_SIG:
         FsmTran(me, motor_fsm_STATE_INITIAL);
         break;
      case TICK_SIG:
         if ((0 != me->autoRestart) &&
               ((ticks - me->lastTimestamp) >= MOTOR_DRV_MIN_STOP_TIME_MS))
         {
            FsmTran(me, motor_fsm_STATE_RUN);
         }
         break;
      default:
         break;
   }
   uint32_t stateId = 3;
   if (e->sig != TICK_SIG)
      LOG_PRINT_INFO(DEBUG_MOTOR, "Motor", "s=%lu;d=%lu;l=%ld;e=%lu", stateId, me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);
}


static void motor_fsm_STATE_ERROR(MotorFsmType *me, Event const *e)
{
   uint32_t ticks = HAL_GetTick();

   switch (e->sig)
   {
      case ENTRY_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "stop", "entry");
         me->lastTimestamp = ticks;

         //stop motor
         motor_stop(me, me->stopType);
         me->pDrvData->curDriveLvl = 0;

         break;

      case EXIT_SIG:
         //Logger_WriteLine("Motor", "s=%s;e=%s", "stop", "exit");
         break;
      case TICK_SIG:

      default:
         break;
   }
   uint32_t stateId = 3;
   if (e->sig != TICK_SIG)
      LOG_PRINT_INFO(DEBUG_MOTOR, "Motor", "s=%lu;d=%lu;l=%ld;e=%lu", stateId, me->pDrvData->curDir, me->pDrvData->curDriveLvl, e->sig);
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

