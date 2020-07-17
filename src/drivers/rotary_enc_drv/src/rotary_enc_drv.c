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
//!   \file       rotary_enc_drv.c
//!
//!   \brief      This is the rotary encoder driver implementation.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "logger_api.h"
#include "mavg4.h"

//********************************************************************
//! @addtogroup rotary_enc_drv_imp
//!   @{
//********************************************************************
#include "rotary_enc_drv_conf.h"
#include "rotary_enc_drv_api.h"
#include "rotary_enc_drv_callouts.h"


//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG   "Encoder"
#define PULSE_INTERVAL_TO_DEGPERSEC(x)    ((90 * 1e6) / ( (x) * ROTARY_ENC_DRV_PPR))
#define PULSE_INTERVAL_TO_DEGPERSEC2(dp,dt)   ((dp * 9 * 1e6) / ( (dt) * (ROTARY_ENC_DRV_PPR/10)))

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct encoder_data_tag
{
   volatile int32_t position;
   volatile uint32_t encATimestamp;
   volatile uint32_t encBTimestamp;
   volatile uint32_t timePeriod;
   volatile uint32_t intCounter;
   volatile uint32_t speed;
   uint32_t lastIntCounter;
   int32_t lastPosition;
   uint32_t lastUpdateTimer;
   uint32_t lastInterval;
   uint32_t lastDeltaPos;
   mavg4Type *filter;
   uint32_t countHistory[5];
   uint32_t oldestHistoryTimestamp;
   uint32_t historySize;
   uint32_t lastDeltaCount;

}encoderDataType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void rotart_enc_drv_gpio_init(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static encoderDataType encoderData;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType RotaryEncDrv_Init(void)
{
   encoderData.position = 0;
   encoderData.encATimestamp = 0;
   encoderData.encBTimestamp = 0;
   encoderData.timePeriod = 0;
   encoderData.intCounter = 0;
   encoderData.lastIntCounter = 0;
   encoderData.lastPosition = 0;
   encoderData.lastUpdateTimer = 0;
   encoderData.lastInterval = 0;
   encoderData.lastDeltaPos = 0;
   encoderData.lastDeltaCount = 0;

   encoderData.oldestHistoryTimestamp = 0;
   encoderData.historySize = 0;

   encoderData.filter = mavg4_create();
   mavg4_init( encoderData.filter);

   rotart_enc_drv_gpio_init();

   return E_OK;
}

StatusType RotaryEncDrv_SetPosition(int32_t newPosition)
{
   encoderData.position = newPosition;
   encoderData.lastPosition = newPosition;

   return E_OK;
}

static inline uint32_t round_closest(uint32_t dividend, uint32_t divisor)
{
    return (dividend + (divisor / 2)) / divisor;
}

void RotaryEncDrv_Update(void)
{
   int32_t currentPos, deltaPos;
   uint32_t timeInterval,now;
//   int16_t newSpeed;
   uint32_t intCounter;
//   int16_t deltaCount;
   uint32_t timePeriod;

   // sample volatile variables, they could change while we execute this routine
   now = RotaryEncDrv_GetHighResTimestamp();
   currentPos = encoderData.position;
   intCounter = encoderData.intCounter;
   timePeriod = encoderData.timePeriod;

   // calculate deltas
//   deltaCount = (intCounter - encoderData.lastIntCounter);
   deltaPos = currentPos - encoderData.lastPosition;
   timeInterval = now - encoderData.lastUpdateTimer;
   //mavg4_writeInput(encoderData.filter, deltaCount);
   //deltaCount = mavg4_readOutput(encoderData.filter);
   //deltaCount = deltaCount >> 8;

   //if (deltaPos > 0)
   //   newSpeed = (timeInterval == 0)? 0: (PULSE_INTERVAL_TO_DEGPERSEC2(deltaPos,timeInterval));
   //else
   //   newSpeed = (timeInterval == 0)? 0: (-1) * (PULSE_INTERVAL_TO_DEGPERSEC2(deltaPos,timeInterval));

   //newSpeed = PULSE_INTERVAL_TO_DEGPERSEC2(deltaCount, timeInterval);

   //mavg4_writeInput(encoderData.filter, newSpeed);
   //encoderData.speed = mavg4_readOutput(encoderData.filter);
   //uint32_t diff = (deltaCount > encoderData.lastDeltaCount)? (deltaCount - encoderData.lastDeltaCount) : (encoderData.lastDeltaCount - deltaCount);
   //uint32_t tmp;
   //if (diff > 1)
   //{
   //   //newSpeed = PULSE_INTERVAL_TO_DEGPERSEC2(deltaCount, 20000);
   //   tmp = deltaCount;//newSpeed;
   //   encoderData.lastDeltaCount = deltaCount;
   //}
   //else
   //{
   //   //newSpeed = PULSE_INTERVAL_TO_DEGPERSEC2(encoderData.lastDeltaCount, 20000);
   //   tmp = encoderData.lastDeltaCount;
   //}
   //if (deltaCount > 0)
   if ((now - encoderData.encATimestamp) < (2 *timeInterval) ||
       (now - encoderData.encBTimestamp) < (2 *timeInterval))
      encoderData.speed = round_closest(1000000UL * 360UL, 2400UL * timePeriod);
   else
      encoderData.speed = 0;

   RotaryEncDrv_OnNewStep(currentPos, deltaPos, encoderData.speed );
   LOG_PRINT_INFO(DEBUG_ENCODER, LOG_TAG, "p=%d;T=%lu;s=%lu", currentPos, encoderData.timePeriod, encoderData.speed );
   encoderData.lastIntCounter = intCounter;

   // update variables
   encoderData.lastPosition = encoderData.position;
   encoderData.lastUpdateTimer = now;
   encoderData.lastInterval = timeInterval;
}

void RotaryEncDrv_IRQHandler(void)
{
   uint32_t counter, encPin, encAEXTI, encBEXTI;
   IOPinStateType encAState, encBState;
   uint32_t timePeriod;

   // EXTI line interrupt detected

   // check the interrupt flags to see if they are associated with our pins
   encAEXTI = __HAL_GPIO_EXTI_GET_IT(IOGetPinNumberFromPinID(ROTARY_ENC_DRV_INPUT_A));
   encBEXTI = __HAL_GPIO_EXTI_GET_IT(IOGetPinNumberFromPinID(ROTARY_ENC_DRV_INPUT_B));

   if ((encAEXTI == 0x00u) && (encBEXTI == 0x00u))
   {
      //it's not our interrupt. we do nothing
      return;
   }

   // it's our interrupt!!
   // retrieve the pin that generated the interrupt
   encPin = (encAEXTI != 0)? IOGetPinNumberFromPinID(ROTARY_ENC_DRV_INPUT_A) : IOGetPinNumberFromPinID(ROTARY_ENC_DRV_INPUT_B);
   // we clear the interrupt flag
   __HAL_GPIO_EXTI_CLEAR_IT(encPin);

   //sample the timer value as soon as possible
   counter = RotaryEncDrv_GetHighResTimestamp();
   //read the pin state to detect the corresponding edge
   encAState = IOReadPinID(ROTARY_ENC_DRV_INPUT_A);
   encBState = IOReadPinID(ROTARY_ENC_DRV_INPUT_B);

   if (encAEXTI != 0)
   {
      if (encAState == IO_ON)
      {
         //rising edge detected
         (encBState == IO_ON)? encoderData.position-- : encoderData.position++;
      }
      else
      {
         //falling edge detected
         (encBState == IO_ON)? encoderData.position++ : encoderData.position--;
      }
      encoderData.encATimestamp = counter;
   }
   else if (encBEXTI != 0)
   {
      if (encBState == IO_ON)
      {
         //rising edge detected
         (encAState == IO_ON)? encoderData.position++ : encoderData.position--;
      }
      else
      {
         //falling edge detected
         (encAState == IO_ON)? encoderData.position-- : encoderData.position++;
      }
      encoderData.encBTimestamp = counter;
   }

   timePeriod = (encoderData.encATimestamp > encoderData.encBTimestamp)?
         encoderData.encATimestamp - encoderData.encBTimestamp:
         encoderData.encBTimestamp - encoderData.encATimestamp;

   encoderData.timePeriod = (encoderData.timePeriod * 4UL + timePeriod) / 5;
   encoderData.intCounter++;


   //Logger_WriteLine("Encoder", "p=%d;T=%lu;s=%lu", encoderData.position, encoderData.timePeriod, speed);
}

int32_t RotaryEncDrv_GetPosition(void)
{
   return encoderData.position;
}

uint32_t RotaryEncDrv_GetSpeed(void)
{
   return (encoderData.timePeriod == 0)? 0: PULSE_INTERVAL_TO_DEGPERSEC(encoderData.timePeriod);
}

uint32_t RotaryEncDrv_GetPeriod(void)
{
   return encoderData.timePeriod;
}

static void rotart_enc_drv_gpio_init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /*Configure GPIO pins*/
   GPIO_InitStruct.Pin = IOGetPinNumberFromPinID(ROTARY_ENC_DRV_INPUT_A);
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(IOGetPortFromPinID(ROTARY_ENC_DRV_INPUT_A), &GPIO_InitStruct);

   GPIO_InitStruct.Pin = IOGetPinNumberFromPinID(ROTARY_ENC_DRV_INPUT_B);
   HAL_GPIO_Init(IOGetPortFromPinID(ROTARY_ENC_DRV_INPUT_B), &GPIO_InitStruct);

   /* EXTI interrupt init*/
   HAL_NVIC_SetPriority(ROTARY_ENC_DRV_IRQ, ROTARY_ENC_DRV_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(ROTARY_ENC_DRV_IRQ);

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

