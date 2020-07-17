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
//!   @file                system_monitor_conf.h
//!
//!   @brief               system monitor configuration header file
//!
//!   @author              Esteban Pupillo
//!
//!   @date                12 MAY 2020
//
//********************************************************************

#ifndef  _SYSTEM_MONITOR_CONF_H
#define  _SYSTEM_MONITOR_CONF_H 1

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

/**
 * Defines the base address of the stack frame.
 * The base address is lowest memory address of the memory region
 * assigned to the stack
 */
#define SYSTEM_MONITOR_BASE_STACK_ADDRESS    0x20000000UL

/**
 * Defines the size of the stack memory region.
 * This size is going to be added to the #SYSTEM_MONITOR_BASE_STACK_ADDRESS
 * to compute the highest memory address that the stack could use
 */
#define SYSTEM_MONITOR_STACK_REGION_SIZE     0x500

/**
 * Defines the minimum free size that is allowed.
 * If the free stack size goes lower than this value an error condition will
 * be generated
 */
#define SYSTEM_MONITOR_MIN_FREE_STACK_SIZE   0x020

/**
 * Defines the time interval between CPU usage reports.
 * This value is expressed in milliseconds
 */
#define SYSTEM_MONITOR_CPU_USAGE_REPORT_PERIOD (1000) //ms

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _MODULE_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

