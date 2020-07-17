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
//!   \file       flow_meter_drv.c
//!
//!   \brief      This is the flow meter driver implementation.
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
#include "logger_api.h"

//********************************************************************
//! \addtogroup 
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "flow_meter_drv_conf.h"
#include "flow_meter_drv_api.h"
#include "flow_meter_drv_callouts.h"


//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG   "Fmeter"
#define PULSE_INTERVAL_TO_MILLILITERPERMIN(x)    ((((5e8 / (x)) + 44) / 825) * 100)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct encoder_data_tag
{
   uint32_t volume;
   uint32_t flowAcc;
   volatile uint32_t lastTimestamp;
   volatile uint32_t timePeriod;
   volatile uint32_t intCounter;
   volatile uint32_t flow;
   uint32_t lastUpdate;
   uint32_t lastIntCount;
   Bool isInitialized;
}flowMeterDataType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void flow_meter_drv_gpio_init(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static flowMeterDataType flowMeterData;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType FlowMeterDrv_Init(void)
{
   flowMeterData.flow = 0;
   flowMeterData.flowAcc = 0;
   flowMeterData.intCounter = 0;
   flowMeterData.lastTimestamp = 0;
   flowMeterData.timePeriod = 0;
   flowMeterData.volume = 0;
   flowMeterData.lastUpdate = 0;
   flowMeterData.isInitialized = FALSE;

   flow_meter_drv_gpio_init();

   flowMeterData.isInitialized = TRUE;

   return E_OK;
}

StatusType FlowMeterDrv_ResetVolume()
{
   flowMeterData.flowAcc = 0;
   flowMeterData.volume = 0;

   return E_OK;
}

void FlowMeterDrv_Update(void)
{
   uint32_t curFlowRate;
   uint32_t now, dt;
   uint32_t intCount;

   now = FlowMeterDrv_GetHighResTimestamp();
   intCount = flowMeterData.intCounter;

   if (flowMeterData.lastIntCount == intCount)
   {
      if ( (now - flowMeterData.lastTimestamp) >= (FLOW_METER_DRV_MAX_MEASURE_TIME_MS * 1000))
      {
         flowMeterData.flow = 0;
      }
   }
   flowMeterData.lastIntCount = intCount;

   curFlowRate = flowMeterData.flow;
   dt = now - flowMeterData.lastUpdate;

   flowMeterData.flowAcc += ((curFlowRate * dt) / (60 * 1e4));
   flowMeterData.volume = flowMeterData.flowAcc / 100;

   flowMeterData.lastUpdate = now;

   LOG_PRINT_INFO(DEBUG_FMETER, LOG_TAG, "f=%lu;v=%lu", flowMeterData.flow, flowMeterData.volume);

   //for debug purposes
   //flowMeterData.intCounter++;
   //flowMeterData.timePeriod = 200000;
   //flowMeterData.flow = PULSE_INTERVAL_TO_MILLILITERPERMIN(flowMeterData.timePeriod);
   //flowMeterData.lastTimestamp = now;
}

void FlowMeterDrv_IRQHandler(void)
{
   uint32_t counter, pinEXTI;

   // EXTI line interrupt detected

   // check the interrupt flags to see if they are associated with our pins
   pinEXTI = __HAL_GPIO_EXTI_GET_IT(IOGetPinNumberFromPinID(FLOW_METER_DRV_INPUT));
   if (pinEXTI == 0x00u)
   {
      //it's not our interrupt. we do nothing
      return;
   }
   // it's our interrupt!!
   // we clear the interrupt flag
   __HAL_GPIO_EXTI_CLEAR_IT(IOGetPinNumberFromPinID(FLOW_METER_DRV_INPUT));

   if(!flowMeterData.isInitialized)
   {
      return;
   }

   //sample the timer value as soon as possible
   counter = FlowMeterDrv_GetHighResTimestamp();

   //calculate the time period
   flowMeterData.timePeriod = counter - flowMeterData.lastTimestamp;
   flowMeterData.lastTimestamp = counter;

   flowMeterData.flow = PULSE_INTERVAL_TO_MILLILITERPERMIN(flowMeterData.timePeriod);

   flowMeterData.intCounter++;
}

uint32_t FlowMeterDrv_GetVolume(void)
{
   return flowMeterData.volume;
}

uint32_t FlowMeterDrv_GetFlowRate(void)
{
   return flowMeterData.flow;
}

static void flow_meter_drv_gpio_init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /*Configure GPIO pins*/
   GPIO_InitStruct.Pin = IOGetPinNumberFromPinID(FLOW_METER_DRV_INPUT);
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(IOGetPortFromPinID(FLOW_METER_DRV_INPUT), &GPIO_InitStruct);

   __HAL_GPIO_EXTI_CLEAR_IT(IOGetPinNumberFromPinID(FLOW_METER_DRV_INPUT));

   /* EXTI interrupt init*/
   HAL_NVIC_SetPriority(FLOW_METER_DRV_IRQ, FLOW_METER_DRV_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(FLOW_METER_DRV_IRQ);

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

