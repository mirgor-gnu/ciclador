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
//!   \file       periodic_callouts_imp.c
//!
//!   \brief      This is the periodic module callouts implementation.
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
#include "board_hw_io.h"
#include "logger_api.h"
#include "motor_drv_api.h"
#include "rotary_enc_drv_api.h"
#include "usart_drv_api.h"
#include "motor_manager_api.h"
#include "adc_drv_api.h"
#include "dflow_meter_drv_api.h"
#include "ventilator_manager_api.h"
#include "clock_drv_api.h"
#include "keyboard_drv_api.h"
#include "alarm_manager_api.h"
#include "power_manager_api.h"
#include "system_monitor_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "periodic_conf.h"
#include "periodic_api.h"
#include "periodic_callouts.h"

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

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************


//********************************************************************
// Function Definitions
//********************************************************************

inline void Periodic_OnProcessingStart(void)
{
   SystemMonitor_StartUserTime();
}

inline void Periodic_OnProcessingStop(void)
{
   SystemMonitor_StopUserTime();
}

void Periodic_handler_1x(void)
{
   //toogle debug led
   //IOTogglePinID(IO_DBG_LED);

   //ADCDrv_StartConversion();
   ADCDrv_Update();

   MotorDrv_Update();
   //IOWritePinID(IO_DBG_LED, IO_ON);
   USARTDrv_Update();
   //IOWritePinID(IO_DBG_LED, IO_OFF);
}

void Periodic_handler_2x(void)
{
   VentilatorMgr_Update();
   AlarmMgr_Update();

   ADCDrv_Dbg();
}

void Periodic_handler_4x(void)
{
   RotaryEncDrv_Update();
   DFlowMeterDrv_Update();
   //FlowMeterDrv_Update();
}

void Periodic_handler_8x(void)
{
   PowerMgr_Update();
   SystemMonitor_Update();
}

void Periodic_handler_16x(void)
{
   KeyboardDrv_Update();
   //RotaryEncDrv_Update();
}

void Periodic_handler_32x(void)
{

}

void Periodic_handler_64x(void)
{

}

void Periodic_handler_128x(void)
{
   //DFlowMeterDrv_Update();
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
