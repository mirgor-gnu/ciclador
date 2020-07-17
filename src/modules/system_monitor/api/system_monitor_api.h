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
//!   @file                system_monitor_api.h
//!
//!   @brief               system monitor APIs header file
//!
//!   @author              Esteban Pupillo
//!
//!   @date                12 MAY 2020
//
//********************************************************************

#ifndef  _SYSTEM_MONITOR_API_H
#define  _SYSTEMO_MONITOR_API_H 1

//********************************************************************
//! @addtogroup system_monitor_api
//!   @{
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
// Remember to use extern modifier
/**
 * Initializes the System Monitor module.
 * This functions shall be called before any other call to this API
 *
 * In order to provide an accurate stack usage it's recommended to
 * call this function directly from the main program entry point where
 * the stack is almost empty.
 *
 * @return #E_OK is initialized successfully\n
 *         #E_ERROR is an error occurred
 */
extern StatusType SystemMonitor_Init(void);

/**
 * System Monitor update.
 * This function shall be called periodically to update the module
 * internal state
 *
 */
extern void SystemMonitor_Update(void);

/**
 * Starts counting for cpu user time.
 * This function shall be called when starting an idle time slice
 */
extern void SystemMonitor_StartUserTime(void);

/**
 * Stops counting for cpu user time.
 * This function shall be called when the last user code was executed
 */
extern void SystemMonitor_StopUserTime(void);

//********************************************************************
// Close the Doxygen group.
//! @}
//********************************************************************
#endif // _SYSTEM_MONITOR_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

