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
//!   \file       Clock_drv.c
//!
//!   \brief      This is the Clock driver implementation file.
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

//********************************************************************
//! @addtogroup clock_drv_imp
//!   @{
//********************************************************************
#include "clock_drv_conf.h"
#include "clock_drv_api.h"
#include "clock_drv_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct clock_drv_data_tag
{
   volatile uint32_t highResTimerH;
   TIM_HandleTypeDef htim;
   volatile uint16_t timerOC;
} ClockDrv_Type;
//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static StatusType clock_drv_timer_init(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static ClockDrv_Type data;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType ClockDrv_Init(void)
{
   StatusType ret = E_ERROR;

   data.highResTimerH = 0;
   data.timerOC = (CLOCK_DRV_TIMER_PERIOD-1);

   if (E_OK != clock_drv_timer_init())
   {
      return ret;
   }

   if (HAL_OK != HAL_TIM_OC_Start_IT(&data.htim, TIM_CHANNEL_1))
   {
      return ret;
   }

   if (HAL_OK != HAL_TIM_Base_Start_IT(&data.htim))
   {
      return ret;
   }



   ret = E_OK;

   return ret;
}

inline uint32_t ClockDrv_GetTicks(void)
{
   return HAL_GetTick();
}

uint32_t ClockDrv_GetHighResTimestamp(void)
{
   uint32_t th, tl1, tl2;

   // get local copy in case we have an interrupt in the middle
   tl1 = CLOCK_DRV_HIGH_RES_TIMER->CNT;
   th = data.highResTimerH;
   tl2 = CLOCK_DRV_HIGH_RES_TIMER->CNT;

   // check is an interrupt has occurred while sampling upper part of the timer
   if (tl2 < tl1)
   {
      th++;
   }

   return ((th << 16) + tl2);
}

//void ClockDrv_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//   if (htim->Instance == data.htim.Instance)
//   {
//      data.highResTimerH++;
//   }
//}

void ClockDrv_IRQHandler(void)
{
   TIM_HandleTypeDef *htim = &data.htim;

   /* TIM Update event */
   if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
   {
      if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) != RESET)
      {
         __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
         // update the upper word
         data.highResTimerH++;
      }
   }
}

void ClockDrv_CCIRQHandler(void)
{
   TIM_HandleTypeDef *htim = &data.htim;

   /* Capture compare 1 event */
   if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC1) != RESET)
   {
      if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC1) != RESET)
      {
         {
            __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
            htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

            if ((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) == 0x00U)
            {
               /* Output compare event */
               data.timerOC += (CLOCK_DRV_TIMER_PERIOD-1);
               htim->Instance->CCR1 = data.timerOC;
               ClockDrv_OnTimerEvent();

            }
            htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
         }
      }
   }
}

StatusType clock_drv_timer_init(void)
{
   TIM_ClockConfigTypeDef sClockSourceConfig = {0};
   TIM_MasterConfigTypeDef sMasterConfig = {0};
   TIM_OC_InitTypeDef sConfigOC = {0};

   data.htim.Instance = CLOCK_DRV_HIGH_RES_TIMER;
   data.htim.Init.Prescaler = CLOCK_DRV_HIGH_RES_TIMER_PRESCALER;
   data.htim.Init.CounterMode = TIM_COUNTERMODE_UP;
   data.htim.Init.Period = 0xFFFF;
   data.htim.Init.ClockDivision = CLOCK_DRV_HIGH_RES_TIMER_CLK_DIV;
   data.htim.Init.RepetitionCounter = 0;
   data.htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
   if (HAL_TIM_Base_Init(&data.htim) != HAL_OK)
   {
      return E_ERROR;
   }

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   if (HAL_TIM_ConfigClockSource(&data.htim, &sClockSourceConfig) != HAL_OK)
   {
      return E_ERROR;
   }

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   if (HAL_TIMEx_MasterConfigSynchronization(&data.htim, &sMasterConfig) != HAL_OK)
   {
      return E_ERROR;
   }

   if (HAL_TIM_OC_Init(&data.htim) != HAL_OK)
   {
      return E_ERROR;
   }

   sConfigOC.OCMode = TIM_OCMODE_ACTIVE;
   sConfigOC.Pulse = data.timerOC;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   if (HAL_TIM_OC_ConfigChannel(&data.htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
   {
      return E_ERROR;
   }

   // enable interrupt
   HAL_NVIC_SetPriority(CLOCK_DRV_HIGH_RES_TIMER_IRQ_NAME, CLOCK_DRV_HIGH_RES_TIMER_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(CLOCK_DRV_HIGH_RES_TIMER_IRQ_NAME);
   HAL_NVIC_SetPriority(CLOCK_DRV_HIGH_RES_TIMER_CC_IRQ_NAME, CLOCK_DRV_HIGH_RES_TIMER_CC_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(CLOCK_DRV_HIGH_RES_TIMER_CC_IRQ_NAME);

   return E_OK;
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

