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
//!   \file       dflow_meter_drv.c
//!
//!   \brief      This is the digital flow meter driver implementation.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       02 Jun 2020
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
#include "dflow_meter_drv_conf.h"
#include "dflow_meter_drv_api.h"
#include "dflow_meter_drv_callouts.h"


//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG   "DFmeter"

#define I2C_INSTANCE_(num)  I2C##num
#define I2C_INSTANCE(num)   I2C_INSTANCE_(num)

#define I2C_EV_IRQ_(num)  I2C##num##_EV_IRQn
#define I2C_EV_IRQ(num)   I2C_EV_IRQ_(num)

#define I2C_ER_IRQ_(num)  I2C##num##_ER_IRQn
#define I2C_ER_IRQ(num)   I2C_ER_IRQ_(num)

#define PULSE_INTERVAL_TO_MILLILITERPERMIN(x)    ((((5e8 / (x)) + 44) / 825) * 100)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef enum sensor_state_tag {
   DFLOW_SENSOR_STATE_READING_SN1,
   DFLOW_SENSOR_STATE_READING_SN2,
   DFLOW_SENSOR_STATE_READING_FLOW,
} DFlowSensorStateType;

typedef struct dflow_meter_data_tag
{
   uint32_t volume;
   uint32_t flowAcc;
   volatile uint32_t lastTimestamp;
   volatile int32_t flow;
   volatile int32_t lastFlow;
   Bool isInitialized;
   I2C_HandleTypeDef  hi2c;
   uint8_t readBuf[4];
   uint32_t serialNumber;
   DFlowSensorStateType sensorState;
   volatile Bool commError;
   volatile Bool readEnqueued;
}DFlowMeterDataType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static StatusType dflow_meter_drv_i2c_init(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static DFlowMeterDataType flowMeterData;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType DFlowMeterDrv_Init(void)
{
   flowMeterData.flow = 0;
   flowMeterData.flow = 0;
   flowMeterData.flowAcc = 0;
   flowMeterData.lastTimestamp = 0;
   flowMeterData.volume = 0;
   flowMeterData.isInitialized = FALSE;
   flowMeterData.serialNumber = 0;
   flowMeterData.sensorState = DFLOW_SENSOR_STATE_READING_SN1;
   flowMeterData.commError = FALSE;
   flowMeterData.readEnqueued = FALSE;

   dflow_meter_drv_i2c_init();

   flowMeterData.isInitialized = TRUE;

   return E_OK;
}

StatusType DFlowMeterDrv_ResetVolume()
{
   flowMeterData.flowAcc = 0;
   flowMeterData.volume = 0;

   return E_OK;
}

void DFlowMeterDrv_Update(void)
{
   LOG_PRINT_INFO(DEBUG_FMETER, LOG_TAG, "f=%ld;v=%lu", flowMeterData.flow, flowMeterData.volume);

   // check if there is a communication error
   if (FALSE != flowMeterData.commError)
   {
      //we found an error. Notify it!
      DFlowMeterDrv_onError();

      //Reset errror flag
      flowMeterData.commError = FALSE;
   }

   // initiate a new read.
   // the function is non-blocking. Once the read is finished the correspoding
   // interrupt callback will be called
   if ((FALSE == flowMeterData.readEnqueued) &&
       (HAL_OK == HAL_I2C_Master_Receive_IT(&flowMeterData.hi2c, DFLOW_METER_DRV_I2C_ADDRESS << 1, flowMeterData.readBuf, 2)))
   {
      flowMeterData.readEnqueued = TRUE;
   }
   else
   {
      //we couldn't start a new read. notify error!
      DFlowMeterDrv_onError();
   }

}

void DFlowMeterDrv_IRQEvHandler(void)
{
   HAL_I2C_EV_IRQHandler(&flowMeterData.hi2c);
}

void DFlowMeterDrv_IRQErHandler(void)
{
   HAL_I2C_ER_IRQHandler(&flowMeterData.hi2c);
}

uint32_t DFlowMeterDrv_GetVolume(void)
{
   return flowMeterData.volume;
}

uint32_t DFlowMeterDrv_GetFlowRate(void)
{
   return flowMeterData.flow;
}

static StatusType dflow_meter_drv_i2c_init(void)
{
   flowMeterData.hi2c.Instance = I2C_INSTANCE(DFLOW_METER_DRV_I2C_CH);
   flowMeterData.hi2c.Mode = HAL_I2C_MODE_MASTER;
   flowMeterData.hi2c.Init.ClockSpeed = 100000;
   flowMeterData.hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
   flowMeterData.hi2c.Init.OwnAddress1 = 0;
   flowMeterData.hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
   flowMeterData.hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
   flowMeterData.hi2c.Init.OwnAddress2 = 0;
   flowMeterData.hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
   flowMeterData.hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

   if (HAL_I2C_Init(&flowMeterData.hi2c) != HAL_OK)
   {
      return E_ERROR;
   }

   // I2C interrupt Init
   HAL_NVIC_SetPriority(I2C_EV_IRQ(DFLOW_METER_DRV_I2C_CH), DFLOW_METER_DRV_I2C_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(I2C_EV_IRQ(DFLOW_METER_DRV_I2C_CH));
   HAL_NVIC_SetPriority(I2C_ER_IRQ(DFLOW_METER_DRV_I2C_CH), DFLOW_METER_DRV_I2C_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(I2C_ER_IRQ(DFLOW_METER_DRV_I2C_CH));

   return E_OK;
}


void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
   // use it only for debug purposes. The logger is not fully thread-safe
   // therefore it can generate problems when called from ISR context
   //Logger_WriteLine(LOG_TAG, "tx cmp;");
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
   uint32_t now;

   now = DFlowMeterDrv_GetHighResTimestamp();

   // use it only for debug purposes. The logger is not fully thread-safe
   // therefore it can generate problems when called from ISR context
   //Logger_WriteLine(LOG_TAG, "r=%lu;%lu;", flowMeterData.readBuf[0], flowMeterData.readBuf[1]);

   if (DFLOW_SENSOR_STATE_READING_SN1 == flowMeterData.sensorState)
   {
      // read first 2 bytes of serial number
      flowMeterData.serialNumber = (flowMeterData.readBuf[0] << 24) + (flowMeterData.readBuf[1] << 16);
      flowMeterData.sensorState = DFLOW_SENSOR_STATE_READING_SN2;
   }
   else if (DFLOW_SENSOR_STATE_READING_SN2 == flowMeterData.sensorState)
   {
      // read first 2 bytes of serial number
      flowMeterData.serialNumber = (flowMeterData.readBuf[0] << 8) + (flowMeterData.readBuf[1] << 0);
      flowMeterData.sensorState = DFLOW_SENSOR_STATE_READING_FLOW;
      flowMeterData.lastTimestamp = 0;
   }
   else if (DFLOW_SENSOR_STATE_READING_FLOW == flowMeterData.sensorState)
   {
      // we are reading flow information
      uint16_t tmp;
      tmp = (flowMeterData.readBuf[0] << 8) + (flowMeterData.readBuf[1] << 0);
      float flow = ((1.0 * DFLOW_METER_SENSOR_FULL_SCALE_FLOW) / 0.8) * (1.0 * tmp / 16384.0 - 0.1 );

      flowMeterData.flow = (int32_t) (flow * 1000.0);
      if ((0 != flowMeterData.lastTimestamp) && (0 < flowMeterData.flow))
      {
         uint32_t dt;
         dt = now - flowMeterData.lastTimestamp;
         flowMeterData.flowAcc += ((flowMeterData.flow * dt) / (60 * 1e4));
         flowMeterData.volume = flowMeterData.flowAcc / 100;
      }
      flowMeterData.lastFlow = flowMeterData.flow;
      flowMeterData.lastTimestamp = now;
   }
   flowMeterData.readEnqueued = FALSE;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
   // use it only for debug purposes. The logger is not fully thread-safe
   // therefore it can generate problems when called from ISR context
   //Logger_WriteLine(LOG_TAG, "e=%lu;", hi2c->ErrorCode);
   flowMeterData.commError = TRUE;
   flowMeterData.readEnqueued = FALSE;
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

