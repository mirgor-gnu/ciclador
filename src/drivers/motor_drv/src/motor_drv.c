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
//!   \file       motor_drv.c
//!
//!   \brief      This is the motor driver implementation file
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

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************


//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static StatusType motor_drv_tim_init(void);
static StatusType motor_drv_gpio_init(void);
volatile uint32_t* motor_drv_get_pwm_register(uint32_t channel);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static MotorDrvType motor_drv_data;
static MotorFsmType motor_fsm;

//********************************************************************
// Function Definitions
//********************************************************************

StatusType MotorDrv_Init(void)
{
   StatusType err = E_ERROR;
   volatile uint32_t *pwmRegister;

   motor_drv_data.isInitialized = FALSE;

   motor_drv_data.homeEvent = -1;

   //init pid controller
   float32_t coef;
   coef = 0.25f;
   arm_float_to_q15(&coef, &motor_drv_data.pid.Kp, 1);
   coef = 0.03f;
   arm_float_to_q15(&coef, &motor_drv_data.pid.Ki, 1);
   coef = 0.00f;
   arm_float_to_q15(&coef, &motor_drv_data.pid.Kd, 1);
   arm_pid_init_q15(&motor_drv_data.pid, 1);

   //motor_drv_data.lpf = lpf_create();
   //lpf_init( motor_drv_data.lpf );


   motor_drv_data.timerPeriod = MOTOR_DRV_FREQ_TO_COUNTS(MOTOR_DRV_PWM_FREQ_HZ);

   pwmRegister = motor_drv_get_pwm_register(MOTOR_DRV_PWM_CHAN_A);
   if (NULL == pwmRegister)
   {
      return E_ERROR;
   }
   motor_drv_data.pwmReg[MOTOR_DRV_CHANNEL_A] = pwmRegister;

   pwmRegister = motor_drv_get_pwm_register(MOTOR_DRV_PWM_CHAN_B);
   if (NULL == pwmRegister)
   {
      return E_ERROR;
   }
   motor_drv_data.pwmReg[MOTOR_DRV_CHANNEL_B] = pwmRegister;

   err = motor_drv_tim_init();
   err = motor_drv_gpio_init();

   motor_drv_data.pFsm = (struct MotorFsmType*)&motor_fsm;
   err = motor_fsm_init(&motor_drv_data);

   motor_drv_data.isInitialized = TRUE;

   return err;
}

StatusType MotorDrv_SetPIDParameters(float32_t kp, float32_t ki, float32_t kd)
{
//   MotorFsmStateType currentState = motor_fsm_get_state(&motor_drv_data);
   //if ((STATE_HOME == currentState) || (STATE_STOP == currentState))
   {
      arm_float_to_q15(&kp, &motor_drv_data.pid.Kp, 1);
      arm_float_to_q15(&ki, &motor_drv_data.pid.Ki, 1);
      arm_float_to_q15(&kd, &motor_drv_data.pid.Kd, 1);
      arm_pid_init_q15(&motor_drv_data.pid, 1);
      return E_OK;
   }

   return E_ERROR;
}
StatusType MotorDrv_GetPIDParameters(float32_t *kp, float32_t *ki, float32_t *kd)
{
   if ((NULL == kp) || (NULL == ki) || (NULL == kd))
   {
      return E_ERROR;
   }

   arm_q15_to_float(&motor_drv_data.pid.Kp, kp, 1);
   arm_q15_to_float(&motor_drv_data.pid.Ki, ki, 1);
   arm_q15_to_float(&motor_drv_data.pid.Kd, kd, 1);

   return E_OK;
}

StatusType MotorDrv_Start(MotorDirType dir, uint32_t driveLevel)
{
   motor_drv_data.newDir = dir;
   motor_drv_data.newDriveLvl = driveLevel;
   motor_drv_data.newDistance = (MOTOR_DRV_MAX_DISTANCE * 2400) / 360;
   motor_drv_data.newSpeed = -1;
   motor_fsm_dispatch(&motor_drv_data, &startEvt);
   return E_OK;
}

StatusType MotorDrv_Stop(MotorStopType stopType)
{
   MotorEventType evt;

   evt.sig = STOP_SIG;
   evt.data = stopType;
   motor_fsm_dispatch(&motor_drv_data, &evt);

   return E_OK;
}

StatusType MotorDrv_MoveDistanceAtSpeed(MotorDirType dir, int32_t distance, int32_t speed)
{
   motor_drv_data.newDir = dir;
   motor_drv_data.newDriveLvl = -1;
   motor_drv_data.newDistance = (distance * 2400) / 360;
   motor_drv_data.newSpeed = speed;
   motor_fsm_dispatch(&motor_drv_data, &startEvt);

   return E_OK;
}

StatusType MotorDrv_MoveDistanceAtDrive(MotorDirType dir, int32_t distance, uint32_t driveLevel)
{
   motor_drv_data.newDir = dir;
   motor_drv_data.newDriveLvl = driveLevel;
   motor_drv_data.newDistance = (distance * 2400) / 360;
   motor_drv_data.newSpeed = -1;
   motor_fsm_dispatch(&motor_drv_data, &startEvt);

   return E_OK;
}

StatusType MotorDrv_ChangeDriveLevel(int32_t driveLevel)
{
   MotorEventType evt;

   if (motor_drv_data.newSpeed > 0)
   {
      // we can't change the drive level is we are controlling speed
      return E_ERROR;
   }
   if (driveLevel < 0)
   {
      //motor_drv_data.newDir = (motor_drv_data.newDir == MOTOR_DIR_CCW)? MOTOR_DIR_CW : MOTOR_DIR_CCW;
      motor_drv_data.newDir = MOTOR_DIR_CW;
      motor_drv_data.newDriveLvl = -driveLevel;
   }
   else
   {
      motor_drv_data.newDir = MOTOR_DIR_CCW;
      motor_drv_data.newDriveLvl = driveLevel;
   }

   evt.sig = UPDATE_DRV_SIG;
   motor_fsm_dispatch(&motor_drv_data, &evt);


   return E_OK;
}


StatusType MotorDrv_GoHome(void)
{
   MotorEventType evt;
   MotorFsmStateType currentState = motor_fsm_get_state(&motor_drv_data);
   if (STATE_HOME != currentState)
   {
      evt.sig = GO_HOME_SIG;
      motor_fsm_dispatch(&motor_drv_data, &evt);
   }
   return E_OK;
}


StatusType MotorDrv_Update(void)
{
   int32_t homeState;
   MotorEventType evt;

   homeState = motor_drv_data.homeEvent;
   motor_drv_data.homeEvent = -1;
   if (homeState != -1)
   {
      evt.sig = HOME_SIG;
      evt.data = homeState;
      motor_fsm_dispatch(&motor_drv_data, &evt);
   }

   motor_fsm_dispatch(&motor_drv_data, (MotorEventType*)&tickEvt);
   return E_OK;
}

void MotorDrv_UpdatePosAndSpeed(int32_t pos, int32_t deltaPos, uint32_t speed)
{
   MotorEventType evt;

   evt.sig = UPDATE_POS_SIG;
   evt.pos = pos;
   evt.speed = speed;

   motor_fsm_dispatch(&motor_drv_data, &evt);
}


void MotorDrv_IRQHandler(void)
{
   HAL_TIM_IRQHandler(&motor_drv_data.htim);
}

void MotorDrv_HomeIRQHandler(void)
{
   uint32_t pinEXTI;

   // EXTI line interrupt detected

   // check the interrupt flags to see if they are associated with our pins
   pinEXTI = __HAL_GPIO_EXTI_GET_IT(IOGetPinNumberFromPinID(MOTOR_DRV_HOME_SWITCH_PIN));
   if (pinEXTI == 0x00u)
   {
      //it's not our interrupt. we do nothing
      return;
   }
   // it's our interrupt!!
   // we clear the interrupt flag
   __HAL_GPIO_EXTI_CLEAR_IT(IOGetPinNumberFromPinID(MOTOR_DRV_HOME_SWITCH_PIN));

   if(!motor_drv_data.isInitialized)
   {
      return;
   }
   if (motor_drv_data.homeEvent < 0)
   {
      motor_drv_data.homeEvent = IOReadPinID(MOTOR_DRV_HOME_SWITCH_PIN);
   }
}


/**
 * @brief Motor driver timer Initialization Function
 * @param None
 * @retval None
 */
static StatusType motor_drv_tim_init(void)
{
   TIM_ClockConfigTypeDef sClockSourceConfig = {0};
   TIM_MasterConfigTypeDef sMasterConfig = {0};
   TIM_OC_InitTypeDef sConfigOC = {0};

   motor_drv_data.htim.Instance = MOTOR_DRV_TIMER;
   motor_drv_data.htim.Init.Prescaler = MOTOR_DRV_TIMER_PRESCALER-1;
   motor_drv_data.htim.Init.CounterMode = TIM_COUNTERMODE_UP;
   motor_drv_data.htim.Init.Period = motor_drv_data.timerPeriod - 1;
   motor_drv_data.htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   motor_drv_data.htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
   if (HAL_TIM_Base_Init(&motor_drv_data.htim) != HAL_OK)
   {
      return E_ERROR;
   }

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   if (HAL_TIM_ConfigClockSource(&motor_drv_data.htim, &sClockSourceConfig) != HAL_OK)
   {
      return E_ERROR;
   }

   if (HAL_TIM_PWM_Init(&motor_drv_data.htim) != HAL_OK)
   {
      return E_ERROR;
   }

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   if (HAL_TIMEx_MasterConfigSynchronization(&motor_drv_data.htim, &sMasterConfig) != HAL_OK)
   {
      return E_ERROR;
   }

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 0;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   if (HAL_TIM_PWM_ConfigChannel(&motor_drv_data.htim, &sConfigOC, MOTOR_DRV_PWM_CHAN_A) != HAL_OK)
   {
      return E_ERROR;
   }

   if (HAL_TIM_PWM_ConfigChannel(&motor_drv_data.htim, &sConfigOC, MOTOR_DRV_PWM_CHAN_B) != HAL_OK)
   {
      return E_ERROR;
   }
   // enable timer interrupts
   HAL_NVIC_SetPriority(MOTOR_DRV_TIMER_IRQ_NAME, MOTOR_DRV_TIMER_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(MOTOR_DRV_TIMER_IRQ_NAME);

   HAL_TIM_Base_Start_IT(&motor_drv_data.htim);
   HAL_TIM_PWM_Start(&motor_drv_data.htim, MOTOR_DRV_PWM_CHAN_A);
   HAL_TIM_PWM_Start(&motor_drv_data.htim, MOTOR_DRV_PWM_CHAN_B);

   return E_OK;
}

static StatusType motor_drv_gpio_init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /*Configure GPIO pins*/
   GPIO_InitStruct.Pin = IOGetPinNumberFromPinID(MOTOR_DRV_HOME_SWITCH_PIN);
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(IOGetPortFromPinID(MOTOR_DRV_HOME_SWITCH_PIN), &GPIO_InitStruct);

   __HAL_GPIO_EXTI_CLEAR_IT(IOGetPinNumberFromPinID(MOTOR_DRV_HOME_SWITCH_PIN));

   /* EXTI interrupt init*/
   HAL_NVIC_SetPriority(MOTOR_DRV_HOME_SWITCH_IRQ, MOTOR_DRV_HOME_SWITCH_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(MOTOR_DRV_HOME_SWITCH_IRQ);

   return E_OK;
}


volatile uint32_t* motor_drv_get_pwm_register(uint32_t channel)
{
   volatile uint32_t *pwmReg = NULL;

   switch(channel)
   {
      case TIM_CHANNEL_1:
         pwmReg = &(MOTOR_DRV_TIMER->CCR1);
         break;
      case TIM_CHANNEL_2:
         pwmReg = &(MOTOR_DRV_TIMER->CCR2);
         break;
      case TIM_CHANNEL_3:
         pwmReg = &(MOTOR_DRV_TIMER->CCR3);
         break;
      case TIM_CHANNEL_4:
         pwmReg = &(MOTOR_DRV_TIMER->CCR4);
         break;
      default:
         break;
   }

   return pwmReg;
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

