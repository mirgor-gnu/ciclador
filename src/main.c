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
//!   \file       main.c
//!
//!   \brief      This is the main entry point of the firmware.
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
#include "clock_drv_api.h"
#include "usart_drv_api.h"
#include "motor_drv_api.h"
#include "rotary_enc_drv_api.h"
#include "periodic_api.h"
#include "logger_api.h"
#include "adc_drv_api.h"
//#include "motor_manager_api.h"
#include "dflow_meter_drv_api.h"
#include "ventilator_manager_api.h"
#include "keyboard_drv_api.h"
#include "display_drv_api.h"
#include "hmi_api.h"
#include "system_monitor_api.h"
#include "power_manager_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************

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
/**
 * @brief Main entry of the program, call all modules initializations and
 *        starts the periodic task.
 *      
 *        This function should never exit.
 * 
 * @return int 
 */
int main(void)
{
  //first init system monitor
  SystemMonitor_Init();

  // init board
  Board_Init();

  // init drivers & modules
  ClockDrv_Init();
  ADCDrv_Init();
  USARTDrv_Init();
  Logger_Init();
  KeyboardDrv_Init();
  // it has to be called before the others because it has a 0.210 seconds delay!
  DisplayDrv_Init();

  MotorDrv_Init();
  RotaryEncDrv_Init();
  DFlowMeterDrv_Init();

  PowerMgr_Init();
  VentilatorMgr_Init();
  Hmi_Init();
  AlarmMgr_Init();

  Periodic_Init();

  // start periodic service
  // this function is blocking
  Periodic_Start();

  // we should never get here!
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
