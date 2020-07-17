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
//!   @file                system_monitor_callouts.h
//!
//!   @brief               system monitor callouts header file
//!
//!   @author              Esteban Pupillo
//!
//!   @date                12 MAY 2020
//
//********************************************************************

#ifndef  _SYSTEM_MONITOR_CALLOUTS_H
#define  _SYSTEM_MONITOR_CALLOUTS_H 1

//********************************************************************
//! @addtogroup system_monitor_callouts
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

/**
 * Get a high resolution timestamp.
 * This function shall return a monotonic timestamp. This timestamp will
 * be used for all the timing calculations
 * @return
 */
extern uint32_t SystemMonitor_GetHighResTimestamp(void);

/**
 * Informs that the free stack size is lower limit.
 * This function is called when the calculated free stack size is
 * lower than the configured (#SYSTEM_MONITOR_MIN_FREE_STACK_SIZE) limit
 *
 * @param freeStack Minimum free stack size
 */
extern void SystemMonitor_OnFreeStackSizeLow(uint32_t freeStack);

/**
 * Informs when a new CPU usage report is available.
 * This functions is called at the time interval defined
 * in #SYSTEM_MONITOR_CPU_USAGE_REPORT_PERIOD
 * @param wallClock Time interval since last report expressed in microseconds.
 * @param CPUUserTime Total CPU time expended in user code
 */
extern void SystemMonitor_OnCPUUsageReport(uint32_t wallClock, uint32_t CPUUserTime);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _SYSTEM_MONITOR_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

