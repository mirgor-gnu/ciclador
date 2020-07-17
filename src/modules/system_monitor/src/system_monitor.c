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
//!   \file       system_monitor.c
//!
//!   \brief      This is the system monitor module implementation file.
//!
//!   \author     Esteban Pupillo
//!
//!   \date       12 MAY 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "logger_api.h"

//********************************************************************
//! @addtogroup system_monitor_imp
//! @{
//********************************************************************

#include "system_monitor_conf.h"
#include "system_monitor_api.h"
#include "system_monitor_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG   "SMON"

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct system_monitor_data_tag
{
   uint32_t stackUsage;
   uint32_t startCPUUserTimestamp;
   uint32_t stopCPUUserTimestamp;
   uint32_t CPUUserTime;
   uint32_t lastCPUUsageReport;
}SystemMonitorDataType;
//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static inline void system_monitor_fill_stack(void);
static inline void system_monitor_check_free_stack(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static SystemMonitorDataType systemMonitorData;

//********************************************************************
// Function Definitions
//********************************************************************

StatusType SystemMonitor_Init(void)
{
   //first of all fill the stack with the magic pattern
   system_monitor_fill_stack();

   //do the first stack verification
   system_monitor_check_free_stack();

   systemMonitorData.CPUUserTime = 0;
   systemMonitorData.lastCPUUsageReport = 0;

   return E_OK;
}

void SystemMonitor_Update(void)
{
   //update stack usage
   system_monitor_check_free_stack();
   LOG_PRINT_INFO(DEBUG_SMON, LOG_TAG, "sf=%lu;", systemMonitorData.stackUsage);

   if (systemMonitorData.stackUsage < SYSTEM_MONITOR_MIN_FREE_STACK_SIZE)
   {
      SystemMonitor_OnFreeStackSizeLow(systemMonitorData.stackUsage);
   }

   uint32_t timestamp = SystemMonitor_GetHighResTimestamp();
   if ((timestamp - systemMonitorData.lastCPUUsageReport) > (SYSTEM_MONITOR_CPU_USAGE_REPORT_PERIOD * 1000))
   {
      uint32_t wallClock = (timestamp - systemMonitorData.lastCPUUsageReport);
      SystemMonitor_OnCPUUsageReport(wallClock, systemMonitorData.CPUUserTime);

      systemMonitorData.CPUUserTime = 0;
      systemMonitorData.lastCPUUsageReport = timestamp;
   }
}

void SystemMonitor_StartUserTime(void)
{
   systemMonitorData.startCPUUserTimestamp = SystemMonitor_GetHighResTimestamp();
}

void SystemMonitor_StopUserTime(void)
{
   uint32_t delta;
   systemMonitorData.stopCPUUserTimestamp = SystemMonitor_GetHighResTimestamp();

   delta = systemMonitorData.stopCPUUserTimestamp - systemMonitorData.startCPUUserTimestamp;

   systemMonitorData.CPUUserTime += delta;
}

static inline void system_monitor_fill_stack(void)
{
   volatile uint32_t var = 0;
   uint32_t *pStart = (uint32_t*)SYSTEM_MONITOR_BASE_STACK_ADDRESS;
   uint32_t *sp = &var;
   uint32_t *p;

   p = pStart;
   while(p < sp)
   {
      *p++ = 0xA5A5A5A5;
   }
}

static inline void system_monitor_check_free_stack(void)
{
   uint32_t *pStart = (uint32_t*) SYSTEM_MONITOR_BASE_STACK_ADDRESS;
   uint32_t *pEnd = pStart + SYSTEM_MONITOR_STACK_REGION_SIZE;
   uint32_t *p;
   uint32_t freeCounter = 0;

   p = pStart;
   while(p < pEnd)
   {
      if (*p++ == 0xA5A5A5A5)
      {
         freeCounter++;
      }
      else
      {
         break;
      }
   }

   systemMonitorData.stackUsage = freeCounter;
}

//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

